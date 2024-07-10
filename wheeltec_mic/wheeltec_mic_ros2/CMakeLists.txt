cmake_minimum_required(VERSION 3.8)
project(wheeltec_mic_ros2)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()


include_directories(
  include
)

link_directories(
  lib/x64
)
#ament_index_register_resource("ros1_bridge_foreign_mapping")
install(DIRECTORY launch config feedback_voice
  DESTINATION share/${PROJECT_NAME})

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(serial REQUIRED)
find_package(wheeltec_mic_msg REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(armquadrover_msgs REQUIRED) 
#星火大模型依赖
find_package(rclcpp_action REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(control_msgs REQUIRED)
INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_SOURCE_DIR}/include/
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # uncomment the line when a copyright and license is not present in all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # uncomment the line when this package is not in a git repo
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

add_executable(spark src/spark.cpp)
ament_target_dependencies(spark rclcpp std_msgs geometry_msgs armquadrover_msgs wheeltec_mic_msg)

install(TARGETS
  spark
  DESTINATION lib/${PROJECT_NAME}
)
target_link_libraries(
   spark
   ${catkin_LIBRARIES} 
   libSparkChain.so -ldl -pthread
   mysqlclient
 )


add_executable(call_recognition src/call_recognition.cpp)
ament_target_dependencies(call_recognition rclcpp std_msgs wheeltec_mic_msg)
install(TARGETS call_recognition
  RUNTIME DESTINATION lib/${PROJECT_NAME}    # 程序运行的时候调用install中的路径
)
install(TARGETS
call_recognition
  DESTINATION lib/${PROJECT_NAME}
)

add_executable(command_recognition src/command_recognition.cpp)
ament_target_dependencies(command_recognition rclcpp std_msgs geometry_msgs)
install(TARGETS command_recognition
  RUNTIME DESTINATION lib/${PROJECT_NAME}    
)
install(TARGETS
command_recognition
  DESTINATION lib/${PROJECT_NAME}
)

add_executable(node_feedback src/node_feedback.cpp)
ament_target_dependencies(node_feedback rclcpp std_msgs)
install(TARGETS node_feedback
  RUNTIME DESTINATION lib/${PROJECT_NAME}   
)
install(TARGETS
node_feedback
  DESTINATION lib/${PROJECT_NAME}
)

add_executable(wheeltec_mic src/wheeltec_mic.cpp src/ros2_service.cpp)
ament_target_dependencies(wheeltec_mic rclcpp std_msgs serial wheeltec_mic_msg)
target_link_libraries(wheeltec_mic -ljsoncpp)
install(TARGETS wheeltec_mic
  RUNTIME DESTINATION lib/${PROJECT_NAME}   
)
install(TARGETS
wheeltec_mic
  DESTINATION lib/${PROJECT_NAME}
)

add_executable(wheeltec_micf src/wheeltec_micf.cpp src/ros2_service.cpp)
ament_target_dependencies(wheeltec_micf rclcpp std_msgs serial wheeltec_mic_msg)
target_link_libraries(wheeltec_micf -ljsoncpp)
install(TARGETS wheeltec_micf
  RUNTIME DESTINATION lib/${PROJECT_NAME}   
)
install(TARGETS
wheeltec_micf
  DESTINATION lib/${PROJECT_NAME}
)

add_executable(voice_control src/voice_control.cpp)
ament_target_dependencies(voice_control rclcpp std_msgs wheeltec_mic_msg)
target_link_libraries(voice_control ${catkin_LIBRARIES} offline_record_lib rt dl pthread stdc++ asound)
install(TARGETS voice_control
  RUNTIME DESTINATION lib/${PROJECT_NAME}   
)
install(TARGETS
voice_control
  DESTINATION lib/${PROJECT_NAME}
)
install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
