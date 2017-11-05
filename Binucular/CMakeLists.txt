
#单独指令编译的话使用下面这条.
# cmake -D CUDA_USE_STATIC_CUDA_RUNTIME=OFF .. //camke command
project(Binocular)
cmake_minimum_required(VERSION 2.8)

#####################################测试部分代码##############################################
#这里设置为调试模式,不然无法调试.
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
#后面添加"include/*.h"是为了在开发环境下能够显示.
file(GLOB SOURCES "src/*.cpp" "inc/*.h" "*.cpp")
include_directories(inc)

set(CUDA_USE_STATIC_CUDA_RUNTIME OFF)
add_executable(test ${SOURCES})
target_link_libraries(test ${OpenCV_LIBS} "-lpopt")

#add_library(camRecall SHARED ${SOURCES})
#target_link_libraries(camRecall ${OpenCV_LIBS} "-lpopt" -fPIC -shared )
