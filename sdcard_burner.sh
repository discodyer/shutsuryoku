#!/bin/bash

# 检查输入参数
if [ "$#" -ne 1 ]; then
  echo "Usage: $0 /dev/sdX"
  exit 1
fi

DEVICE=$1
BOOT_IMG_URL="https://mirror.iscas.ac.cn/openeuler-sig-riscv/openEuler-RISC-V/devel/20240916/v0.1/meles/boot-20240916-045848.ext4.zst"
ROOT_IMG_URL="https://mirror.iscas.ac.cn/openeuler-sig-riscv/openEuler-RISC-V/devel/20240916/v0.1/meles/root-20240916-045848.ext4.zst"
BOOT_IMG="boot-20240916-045848.ext4.zst"
ROOT_IMG="root-20240916-045848.ext4.zst"

# 检查设备是否存在
if [ ! -b "$DEVICE" ]; then
  echo "Error: Device $DEVICE not found."
  exit 1
fi

# 下载boot和root镜像
if [ ! -f "$BOOT_IMG" ]; then
  echo "Downloading boot image from $BOOT_IMG_URL..."
  wget -O "$BOOT_IMG" "$BOOT_IMG_URL"
  if [ $? -ne 0 ]; then
    echo "Error downloading boot image."
    exit 1
  fi
fi

if [ ! -f "$ROOT_IMG" ]; then
  echo "Downloading root image from $ROOT_IMG_URL..."
  wget -O "$ROOT_IMG" "$ROOT_IMG_URL"
  if [ $? -ne 0 ]; then
    echo "Error downloading root image."
    exit 1
  fi
fi

# 1. 取消挂载SD卡所有已挂载的分区
echo "Unmounting all mounted partitions on $DEVICE..."
for part in $(ls ${DEVICE}*? 2>/dev/null); do
  sudo umount $part 2>/dev/null || true
done

# 2. 清理分区表
echo "Creating new GPT partition table on $DEVICE..."
sudo parted -s $DEVICE mklabel gpt

# 3. 创建第一个分区 (2MB, Linux Data partition)
echo "Creating Linux Data partition (2MB)..."
sudo parted -s $DEVICE mkpart primary 1MiB 3MiB

# 4. 创建第二个分区 (500MB, EXT4, Boot partition)
echo "Creating Boot partition (500MB)..."
sudo parted -s $DEVICE mkpart primary ext4 3MiB 503MiB

# 5. 创建第三个分区 (4GB, Linux Swap partition)
echo "Creating Swap partition (4GB)..."
sudo parted -s $DEVICE mkpart primary linux-swap 503MiB 4503MiB

# 6. 创建第四个分区 (6GB, EXT4, Root partition)
echo "Creating Root partition using remaining space..."
sudo parted -s $DEVICE mkpart primary ext4 4503MiB 100%

# 7. 格式化分区
echo "Formatting partitions..."
sudo mkfs.ext4 ${DEVICE}2
sudo mkswap ${DEVICE}3
sudo mkfs.ext4 ${DEVICE}4

# 8. 解压并复制boot文件系统到第二个分区
echo "Extracting and copying boot filesystem to ${DEVICE}2..."
BOOT_MOUNT=/mnt/boot
sudo mkdir -p $BOOT_MOUNT
sudo mount ${DEVICE}2 $BOOT_MOUNT
sudo zstdcat $BOOT_IMG | sudo dd of=${DEVICE}2 bs=4M status=progress
sync
sudo umount $BOOT_MOUNT

# 9. 解压并复制root文件系统到第四个分区
echo "Extracting and copying root filesystem to ${DEVICE}4..."
ROOT_MOUNT=/mnt/root
sudo mkdir -p $ROOT_MOUNT
sudo mount ${DEVICE}4 $ROOT_MOUNT
sudo zstdcat $ROOT_IMG | sudo dd of=${DEVICE}4 bs=4M status=progress
sync
sudo umount $ROOT_MOUNT

# 10. 确保第四个分区未挂载，运行e2fsck检查文件系统
echo "Running e2fsck on ${DEVICE}4..."
sudo umount ${DEVICE}4 2>/dev/null || true
sudo e2fsck -f ${DEVICE}4

# 11. 扩展root分区文件系统以使用所有可用空间
echo "Resizing filesystem on ${DEVICE}4..."
sudo resize2fs ${DEVICE}4

echo "SD card setup complete!"
