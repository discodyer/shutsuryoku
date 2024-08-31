#include "rclcpp/rclcpp.hpp"
#include "turtlesim/msg/pose.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include <cmath>

const double ANGULAR_SPEED = 0.2;        // 控制乌龟转向时的角速度
const double LINEAR_SPEED = 0.5;         // 控制乌龟前进时的线速度
const double ANGULAR_THRESHOLD = 0.02;   // 角度误差阈值，用于判断是否需要继续转向，单位是弧度
const double DISTANCE_THRESHOLD = 0.1;   // 距离误差阈值，用于判断是否到达目标点

class TurtleRectangularMover : public rclcpp::Node
{
public:
    TurtleRectangularMover()
        : Node("turtle_rectangular_mover")
    {
        // 订阅 /turtle1/pose 话题，获取小乌龟的位置和姿态信息
        pose_subscription_ = this->create_subscription<turtlesim::msg::Pose>(
            "/turtle1/pose", 10, std::bind(&TurtleRectangularMover::pose_callback, this, std::placeholders::_1));

        // 发布 /turtle1/cmd_vel 话题，用于控制小乌龟的移动
        velocity_publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);

        // 初始化移动到第一个目标点
        move_to_next_point();
    }

private:
    void pose_callback(const turtlesim::msg::Pose::SharedPtr msg)
    {
        // 更新当前乌龟的位置和朝向
        current_pose_ = *msg;
        print_odom_info();  // 打印用于调试的位置信息

        // 计算乌龟到目标点的距离和角度误差
        double distance_to_goal = std::sqrt(std::pow(goal_x_ - current_pose_.x, 2) + std::pow(goal_y_ - current_pose_.y, 2));
        double angle_to_goal = std::atan2(goal_y_ - current_pose_.y, goal_x_ - current_pose_.x);
        double angular_error = normalize_angle(angle_to_goal - current_pose_.theta);

        geometry_msgs::msg::Twist twist;

        // 如果角度误差较大，先转向目标方向
        if (std::abs(angular_error) > ANGULAR_THRESHOLD)
        {
            twist.linear.x = 0.0;  // 停止前进，先调整角度
            twist.angular.z = angular_error > 0 ? ANGULAR_SPEED : -ANGULAR_SPEED;
        }
        // 当角度误差足够小时，开始向目标前进
        else if (distance_to_goal > DISTANCE_THRESHOLD)
        {
            twist.linear.x = LINEAR_SPEED;
            twist.angular.z = 0.0;  // 直线前进时不转向
        }
        // 如果到达目标点，移动到下一个点
        else
        {
            move_to_next_point();  // 移动到下一个矩形点
        }

        velocity_publisher_->publish(twist);  // 发布速度指令
    }

    // 确保角度在 [-pi, pi] 范围内
    double normalize_angle(double angle)
    {
        while (angle > M_PI) angle -= 2 * M_PI;
        while (angle < -M_PI) angle += 2 * M_PI;
        return angle;
    }

    // 切换到下一个目标点
    void move_to_next_point()
    {
        // 定义矩形的四个顶点
        static std::vector<std::pair<double, double>> corners = {{2.0, 2.0}, {8.0, 2.0}, {8.0, 8.0}, {2.0, 8.0}};
        current_goal_index_ = (current_goal_index_ + 1) % corners.size();  // 循环切换到下一个顶点
        goal_x_ = corners[current_goal_index_].first;
        goal_y_ = corners[current_goal_index_].second;
    }

    // 打印当前乌龟的位置信息用于调试
    void print_odom_info()
    {
        RCLCPP_INFO(this->get_logger(), "Turtle position: [x: %f, y: %f, theta: %f]", current_pose_.x, current_pose_.y, current_pose_.theta);
    }

    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr pose_subscription_;  // 订阅乌龟姿态话题
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr velocity_publisher_;  // 发布速度指令

    double goal_x_ = 2.0;  // 当前目标点的 x 坐标
    double goal_y_ = 2.0;  // 当前目标点的 y 坐标
    int current_goal_index_ = 0;  // 当前目标点在矩形顶点中的索引

    turtlesim::msg::Pose current_pose_;  // 当前乌龟的位置信息
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<TurtleRectangularMover>());
    rclcpp::shutdown();
    return 0;
}
