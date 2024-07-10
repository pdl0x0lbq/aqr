cmake_minimum_required(VERSION 3.8)
project(armquadrover)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)

find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(control_msgs REQUIRED)
find_package(armquadrover_msgs REQUIRED)
INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_SOURCE_DIR}/include/ArmQuadRover/
)

# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()


ament_package()
add_executable(node_01 src/quardc.cpp)
add_executable(ArmServer src/ArmServer.cpp src/Action.cpp)
add_executable(SerialNode src/SerialNode.cpp src/servo.cpp)
ament_target_dependencies(node_01 rclcpp std_msgs geometry_msgs armquadrover_msgs)
ament_target_dependencies(ArmServer rclcpp std_msgs geometry_msgs control_msgs sensor_msgs  rclcpp_action armquadrover_msgs)
ament_target_dependencies(SerialNode rclcpp std_msgs armquadrover_msgs sensor_msgs )

install(TARGETS
  node_01
  ArmServer
  SerialNode
  DESTINATION lib/${PROJECT_NAME}
)
