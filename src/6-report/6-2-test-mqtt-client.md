## 测试mqtt-client

### mqtt-client

[这个仓库](https://github.com/ros2-gbp/mqtt_client-release/)是存放ROS各个版本的release版本代码的

[这个仓库](https://github.com/ika-rwth-aachen/mqtt_client)是上游仓库，是最新的源码

mqtt_client 包提供了 ROS Nodelet 或 ROS 2 组件节点，使连接的基于 ROS 的设备或机器人能够使用 MQTT 协议通过 MQTT 代理交换 ROS 消息。这通常适用于任意 ROS 消息类型。 mqtt_client 还可以与运行在不基于 ROS 的设备上的 MQTT 客户端交换原始消息。

参考上游仓库的readme,需要先启动一个MQTT服务器，比如mosquitto，然后再修改yaml参数文件

### Eulermaker

[这个地址](https://eulermaker.compass-ci.openeuler.openatom.cn/project/overview?osProject=openEuler_24.03_LTS:paho-mqtt)是我目前在Eulermaker上打包成功的仓库

你需要先启动一个openEuler 24.03的虚拟机，然后按照[ros文档安装ROS2 humble](https://openeuler-ros-docs.readthedocs.io/en/latest/installation/install-ros-humble.html)

然后修改/etc/yum.repos.d/ROS.repo文件

```bash
sudo nano /etc/yum.repos.d/ROS.repo
```

添加下面的内容

```
[mqtt-client]
name=mqtt-client
baseurl=https://eulermaker.compass-ci.openeuler.openatom.cn/api/ems2/repositories/openEuler_24.03_LTS:paho-mqtt/openEuler%3A24.03-LTS/x86_64/
enabled=1
gpgcheck=0
```

然后再执行`sudo dnf update`更新软件源

然后就可以安装软件包了

```
sudo dnf install ros-humble-mqtt-client
```

然后应该就会安装下面四个包

- ros-humble-mqtt-client
- ros-humble-mqtt-client-interfaces
- paho-mqtt-devel
- paho-mqttpp-devel
