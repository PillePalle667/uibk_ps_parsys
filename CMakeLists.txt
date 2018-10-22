cmake_minimum_required(VERSION 3.12)
project(uibk_ps_parsys)

set(CMAKE_CXX_STANDARD 14)

include_directories(exercise02)

add_executable(uibk_ps_parsys
        exercise01/mmul.cpp
        exercise01/mmul_loopunroll.cpp
        exercise01/mmulOpenMP.cpp
        exercise01/mmulOpt.cpp
        exercise02/chrono_timer.h
        exercise02/mmul.cpp)
