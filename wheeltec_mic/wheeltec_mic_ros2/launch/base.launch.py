import os
from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():
    mic_dir = get_package_share_directory('wheeltec_mic_ros2')
    audio_path = os.path.join(mic_dir,'feedback_voice')
    print(audio_path)

    call_recognition = Node(
        package="wheeltec_mic_ros2",
        executable="call_recognition",
        #output='screen',
        parameters=[{"confidence_threshold": 20, 	#离线命令词识别置信度
        	"time_per_order": 10}]					#最大识别录音持续时长(0~10s)
    )
    command_recognition = Node(
        package="wheeltec_mic_ros2",
        executable="command_recognition",
        output='screen',
        parameters=[{"audio_path": audio_path}]
    )
    node_feedback = Node(
        package="wheeltec_mic_ros2",
        executable="node_feedback",
        output='screen',
        parameters=[{"audio_path": audio_path}]
    )



    ld = LaunchDescription()

    ld.add_action(call_recognition)
    ld.add_action(command_recognition)
    ld.add_action(node_feedback)
  
    
    return ld