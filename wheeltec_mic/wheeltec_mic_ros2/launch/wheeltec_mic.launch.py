import os
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, ExecuteProcess
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    # 声明 launch 参数
    usart_port_name_arg = DeclareLaunchArgument(
        'usart_port_name',
        default_value='/dev/wheeltec_mic',
        description='USART 端口的名称'
    )

    serial_baud_rate_arg = DeclareLaunchArgument(
        'serial_baud_rate',
        default_value='115200',
        description='串行通信的波特率'
    )

    # 定义节点
    wheeltec_mic_node = Node(
        package='wheeltec_mic_ros2',
        executable='wheeltec_mic',
        name='wheeltec_mic',
        output='screen',
        parameters=[
            {'usart_port_name': LaunchConfiguration('usart_port_name')},
            {'serial_baud_rate': LaunchConfiguration('serial_baud_rate')}
        ]
    )

    return LaunchDescription([
        usart_port_name_arg,
        serial_baud_rate_arg,
        wheeltec_mic_node
    ])