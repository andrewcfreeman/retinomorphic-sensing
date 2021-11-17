cmake_minimum_required(VERSION 3.19)
project(Simulator)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

find_package( OpenCV REQUIRED PATHS "/home/andrew/Programs/OpenCV/build/")

add_executable(Simulator
        ATISSimulator.cpp
        bitbuffer.cpp
        bitbuffer.h
        CeleXSimulator.cpp
        DAVISSimulator.cpp
        FSMSimulator.cpp
        getopt.c
        getopt.h
        parse.h
        RetinaSimulator.cpp
        run.cpp
        Simulatorbase.h
        VidarSimulator.cpp)

target_include_directories(Simulator PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(Simulator stdc++fs ${OpenCV_LIBS})
