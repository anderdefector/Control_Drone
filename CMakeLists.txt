cmake_minimum_required(VERSION 2.8.3)
project(control_drone)

add_compile_options(-std=c++11)

find_package(OpenCV REQUIRED)

find_package(catkin REQUIRED COMPONENTS
	bebop_msgs
	cv_bridge
  	image_transport
  	roscpp
  	sensor_msgs
  	std_msgs
)

catkin_package()
include_directories(
# include
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}	
)


add_executable(Keyboard src/key_con.cpp)
target_link_libraries(Keyboard ${catkin_LIBRARIES})

add_executable(C_P1 src/c_p1.cpp)
target_link_libraries(C_P1 ${catkin_LIBRARIES})

add_executable(L_Color src/l_color.cpp)
target_link_libraries(L_Color ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})

add_executable(L_BN src/l_bn.cpp)
target_link_libraries(L_BN ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})

add_executable(Pelota src/Pelota.cpp)
target_link_libraries(Pelota ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})

add_executable(Cuadro_Color src/cuadro_c.cpp)
target_link_libraries(Cuadro_Color ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})

add_executable(C_P2 src/c_p2.cpp)
target_link_libraries(C_P2 ${catkin_LIBRARIES})

add_executable(C_P2v src/c_p2v.cpp)
target_link_libraries(C_P2v ${catkin_LIBRARIES})

add_executable(C_PT src/c_pt.cpp)
target_link_libraries(C_PT ${catkin_LIBRARIES})

add_executable(C_P5 src/c_p5.cpp)
target_link_libraries(C_P5 ${catkin_LIBRARIES})

add_executable(C_P6 src/c_p6.cpp)
target_link_libraries(C_P6 ${catkin_LIBRARIES})

add_executable(vi_wp src/vi_waypoints.cpp)
target_link_libraries(vi_wp ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})

add_executable(C_VY src/c_vy.cpp)
target_link_libraries(C_VY ${catkin_LIBRARIES})

add_executable(C_VAZ src/c_vaz.cpp)
target_link_libraries(C_VAZ ${catkin_LIBRARIES})

add_executable(C_VZ src/c_vz.cpp)
target_link_libraries(C_VZ ${catkin_LIBRARIES})
