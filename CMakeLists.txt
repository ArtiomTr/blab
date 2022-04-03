cmake_minimum_required(VERSION 3.20)
project(blab)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(blab ./src/meter.cpp ./src/metric.cpp ./src/not_implemented.cpp ./src/environment.cpp ./src/report.cpp)

if(DEFINED ${COMPILE_EXAMPLE})
    message("Building example")
    add_executable(test ./main.cpp)
    target_link_libraries(test blab)
endif()
