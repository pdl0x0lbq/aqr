cmake_minimum_required(VERSION 3.8)
project(xf_asr)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(armquadrover_msgs REQUIRED) 
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)
include_directories(include)

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

add_executable(xf_asr src/xf_asr.cpp src/speech_recognizer.c src/linuxrec.c)
ament_target_dependencies(xf_asr rclcpp std_msgs armquadrover_msgs)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

target_link_libraries(xf_asr
  ${PROJECT_SOURCE_DIR}
  libmsc1.so -ldl -lpthread -lm -lrt -lasound
)

install(TARGETS
  xf_asr
  DESTINATION lib/${PROJECT_NAME}
)
install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
