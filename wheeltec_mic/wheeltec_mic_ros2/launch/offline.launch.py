import launch
from launch import LaunchDescription
import launch.actions
import launch_ros.actions

def generate_launch_description():
    return launch.LaunchDescription([
        launch.actions.ExecuteProcess(
            cmd=['gnome-terminal', '--disable-factory', '--', 'bash', '-c', 'ros2 run wheeltec_mic_ros2 wheeltec_mic --ros-args -p usart_port_name:=/dev/wheeltec_mic -p serial_baud_rate:=115200; echo "Press any key to continue..."; read -n 1'],
            output='screen'
        ),
        launch.actions.ExecuteProcess(
            cmd=['gnome-terminal', '--disable-factory', '--', 'bash', '-c', 'ros2 run wheeltec_mic_ros2 voice_control --ros-args -p appid:=9d131769; echo "Press any key to continue..."; read -n 1'],
            output='screen'
        )
    ])
