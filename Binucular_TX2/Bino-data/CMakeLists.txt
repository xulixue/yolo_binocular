

# cmake -D CUDA_USE_STATIC_CUDA_RUNTIME=OFF .. //camke command
project(makelib)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_BUILD_TYPE "Debug")
if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fpermissive -g -Wall -Wno-unused-variable -pthread")
else( CMAKE_BUILD_TYPE STREQUAL "Debug" )
        set(CMAKE_BUILD_TYPE "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -pthread -fopenmp")
endif( CMAKE_BUILD_TYPE STREQUAL "Debug" )
message("Build Type:"${CMAKE_BUILD_TYPE} ${CMAKE_CXX_FLAGS})

set(OpenCV_DIR /usr/local/opencv2.4.8/share/OpenCV)
find_package( OpenCV REQUIRED )

file(GLOB SOURCES "src/*.cpp" "inc/*.h" "*.cpp")
include_directories(inc)

add_executable (tst testBinocular.cpp)
target_link_libraries(tst  ${OpenCV_LIBS} "libSelDepth.so")
