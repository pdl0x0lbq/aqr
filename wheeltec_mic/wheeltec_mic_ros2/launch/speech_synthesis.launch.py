#!/usr/bin/env python3
import launch
from launch_ros.actions import Node

def generate_launch_description():
    # 创建一个启动描述的列表
    launch_description = []

    # 添加 toycar_voice 包的节点
    node2 = Node(
        package='toycar_voice',
        executable='listener',
        name='speech_synthesis',
        output='screen'
    )
    launch_description.append(node2)

    # 返回启动描述
    return launch.LaunchDescription(launch_description)
