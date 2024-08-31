#!/usr/bin/env python

import rclpy
from rclpy.node import Node
from turtlesim.srv import Spawn

class TurtleSpawner(Node):
    def __init__(self):
        super().__init__('turtle_spawner')
        # 创建一个客户端用于调用 /spawn 服务
        self.client = self.create_client(Spawn, '/spawn')

        # 等待服务就绪
        while not self.client.wait_for_service(timeout_sec=1.0):
            self.get_logger().info('等待 /spawn 服务就绪...')

        # 创建请求并设置参数
        self.request = Spawn.Request()
        self.request.x = 2.0
        self.request.y = 2.0
        self.request.theta = 0.2
        self.request.name = ''  # 如果留空，系统将自动生成一个名字

        # 异步发送请求
        self.future = self.client.call_async(self.request)
        self.future.add_done_callback(self.callback_spawn_response)

    def callback_spawn_response(self, future):
        try:
            response = future.result()
            self.get_logger().info(f'乌龟已生成，名称: {response.name}')
        except Exception as e:
            self.get_logger().error(f'服务调用失败: {str(e)}')

def main(args=None):
    rclpy.init(args=args)

    turtle_spawner = TurtleSpawner()

    rclpy.spin(turtle_spawner)

    turtle_spawner.destroy_node()
    
    rclpy.shutdown()

if __name__ == '__main__':
    main()
