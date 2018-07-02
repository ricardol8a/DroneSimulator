cmake_minimum_required(VERSION 2.8.3)
project(simulator)

add_compile_options(-std=c++11 -O3 -pthread -DCONFIG=\"${CMAKE_CURRENT_SOURCE_DIR}/config/\" -DDATA=\"${CMAKE_CURRENT_SOURCE_DIR}/data/\" -DMODEL=REAL)

find_package(catkin REQUIRED COMPONENTS roscpp geometry_msgs std_msgs tf2_msgs ardrone_autonomy)

catkin_package()

include_directories(include ${catkin_INCLUDE_DIRS})

link_directories(lib)

add_executable(setup src/main.cpp src/base.cpp src/camera.cpp src/clock.cpp src/control.cpp src/controller.cpp src/filter.cpp src/graphics.cpp src/info.cpp src/inputs.cpp src/kernel.cpp src/model.cpp src/obstacle.cpp src/output.cpp src/plane.cpp src/link.cpp src/register.cpp src/robot.cpp src/scene.cpp src/sector.cpp src/space.cpp src/system.cpp src/timer.cpp src/trajectory.cpp src/window.cpp)

target_link_libraries(setup ${catkin_LIBRARIES} m glut GLU GL)
