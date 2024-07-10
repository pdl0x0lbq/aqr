cmake_minimum_required(VERSION 3.8)
project(toycar_voice)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(std_msgs REQUIRED)
find_package(rclcpp REQUIRED)

add_executable(talker src/publisher_member_function.cpp)
ament_target_dependencies(talker 
rclcpp
std_msgs
)

add_executable(listener src/subscriber_member_function.cpp)
target_include_directories(listener PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_include_directories(talker PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_compile_features(listener PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17

ament_target_dependencies(
  listener
  "std_msgs"
  "rclcpp"
)
# target_link_libraries("libmsc.so" msc)
target_link_libraries(listener
  ${link_directories}
  libmsc1.so -ldl -pthread 
)
target_link_libraries(talker
  ${link_directories}
  libmsc1.so -ldl -pthread 
)
install(TARGETS talker
  DESTINATION lib/${PROJECT_NAME})
install(TARGETS listener
  DESTINATION lib/${PROJECT_NAME})

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
