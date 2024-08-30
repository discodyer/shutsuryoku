# ROS基础编程测试题目

## test 01

 - 启动小乌龟仿真界面
 - 新建功能包，编写第一个ros2节点实现话题发布功能，通过发布Twist消息让小乌龟跑一个长方形轨迹（基于odom信息反馈）
 - 编写第二个ros2节点实现服务调用功能：即将下列命令行实现的效果改为编程调用python服务实现  
`ros2 service call /spawn turtlesim/srv/Spawn "{x: 2, y: 2, theta: 0.2, name: ''}"`

## test 02

 - 新建功能包，编写第一个ROS节点实现TF发布功能：发布/world_link到/base_link的静态TF（可自定义）以及/base_link到/camera_link的动态TF（可自定义）
 - 编写第二个ROS节点实现TF监听功能：监听/camera_link到/base_link的动态TF并在终端打印输出
 - 启动rviz查看TF的实时变化

## test 03

 - 新建功能包，设计一个名为 PerformMathOperation 的服务接口，该接口用于执行基本的数学运算，包括加法、减法、乘法和除法。服务的请求为两个数num和需要操作的类型（加法、减法、乘法和除法，服务的返回为计算结果
 - 分别编写服务端节点和客户端节点实现上面的功能
 - 客户端允许在启动时以参数的形式输入两个num的值和需要操作的类型， 当接收到服务返回的结果后，将结果以话题的形式发布出来

## test 04

 - 新建功能包，自定义action通信接口让小乌龟走一条直线并反馈执行过程和结果。action的goal为start_move，feedback为小乌龟的odom信息（可以设置一定的更新频率），goal为小乌龟是否完成目标（走完自定义的一段路程）
 - 编写action的服务端和客户端程序

## test 05

 - 安装usb_cam或其他USB摄像头驱动包驱动笔记本电脑自带的摄像头
 - 新建功能包，编写节点实时订阅摄像头发布的图像话题消息并将ROS图像消息转换为OpenCV图像
 - 在图像右上角绘制矩形，再将OpenCV图像转换回ROS图像消息重新发布到一个新的话题
 - 用rviz或者rqt_image_view显示图像消息

代码放在 [这个仓库](https://github.com/discodyer/ros2playground) ，相关说明文档会放在这边