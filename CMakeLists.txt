

cmake_minimum_required(VERSION 2.8.4)
PROJECT(QLearning)
add_definitions(-DDQLA_FP_TYPE=float)
include(/usr/include/dlib/cmake)


set(source_files
	main.cpp
)

set(source_2048
	experiences/2048/grid.cpp
	experiences/2048/utils.cpp
        experiences/2048/experience.cpp
)
ADD_EXECUTABLE(program ${source_files} ${source_2048})

ADD_EXECUTABLE(gametest ${source_2048} experiences/2048/main.cpp)
TARGET_LINK_LIBRARIES(program dlib)
