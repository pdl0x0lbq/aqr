from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='depth_image_processing',
            executable='depth_image_processing',
            name='depth_image_processing',
            output='screen',
            parameters=[{'param_name': 'param_value'}]  # 添加节点参数
        )
    ])
