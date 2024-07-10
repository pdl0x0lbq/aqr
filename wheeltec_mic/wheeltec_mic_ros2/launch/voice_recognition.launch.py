import launch
from launch_ros.actions import Node
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource

def generate_launch_description():
    # 创建一个启动描述的列表
    launch_description = []

    # 添加 xf_asr 包的节点
    node1 = Node(
        package='xf_asr',
        executable='xf_asr',
        output='screen'
    )
    launch_description.append(node1)


    # 返回启动描述
    return launch.LaunchDescription(launch_description)
