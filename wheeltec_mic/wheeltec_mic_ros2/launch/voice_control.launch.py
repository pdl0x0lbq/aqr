from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    voice_control = Node(
        package="wheeltec_mic_ros2",
        executable="voice_control",
        output='screen',
        parameters=[{"appid": "9d131769"}]
    )

    return LaunchDescription([voice_control])
