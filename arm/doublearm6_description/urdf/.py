#!/usr/bin/env python3

import rclpy
from rclpy.node import Node

def main():
    rclpy.init()
    node = Node("urdf_reader_node")

    # URDF 文件路径
    urdf_file = "/home/swen/urdf_ws/src/doublearm6_description/urdf/doublearm6.urdf"

    # 加载 URDF 模型
    try:
        with open(urdf_file, 'r') as urdf_file_handle:
            urdf_xml = urdf_file_handle.read()
    except IOError:
        node.get_logger().error("Failed to open URDF file")
        return

    # 打印 URDF 模型信息示例
    node.get_logger().info("URDF content:\n%s" % urdf_xml)

    # 进行其他操作，如解析和处理 URDF 内容

    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == '__main__':
    main()
