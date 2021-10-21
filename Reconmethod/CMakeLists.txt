cmake_minimum_required(VERSION 3.19)
project(Reconmethod)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

find_package( OpenCV REQUIRED PATHS "/home/andrew/Programs/OpenCV/build/")

add_executable(Reconmethod
        getopt.c
        getopt.h
        parse.h
        ReconATIS.cpp
        Reconbase.h
        ReconCeleX.cpp
        ReconDAVIS.cpp
        ReconFSM.cpp
        Reconmain.cpp
        ReconRetina.cpp DVSToADDERTranscoder.cpp DVSToADDERTranscoder.h)

target_include_directories(Reconmethod PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(Reconmethod stdc++fs ${OpenCV_LIBS})

