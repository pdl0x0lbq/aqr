from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node

def generate_launch_description():
    # 添加一个参数来接收用户输入的 choice 值
    choice_arg = DeclareLaunchArgument(
        'choice',
        default_value='1',  # 默认值为 1
        description='1 代表在线，0 代表离线，请输入 choice 的值'
    )

    wheeltec_mic = Node(
        package="wheeltec_mic_ros2",
        executable="wheeltec_mic",
        output='screen',
        parameters=[{"usart_port_name": "/dev/wheeltec_mic",
                    "serial_baud_rate": 115200,
                    # 使用 Substitution 获取用户输入的 choice 值
                    "choice": LaunchConfiguration('choice')}]
    )

    # 将参数添加到 LaunchDescription 中
    return LaunchDescription([choice_arg, wheeltec_mic])
