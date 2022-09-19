cmake_minimum_required(VERSION 3.22)
project(test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(OpenCV_DIR "opencv/lib/cmake/opencv4")
set(libx64_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/libx264/lib/libx264.a")
find_package(OpenCV REQUIRED)

add_compile_options(
    -O3
    -ffast-math
    -pthread
)

add_link_options(
    -pthread
    -O3
    -sPTHREAD_POOL_SIZE=navigator.hardwareConcurrency
    -ffast-math
    -sEXPORTED_FUNCTIONS=_initWriter,_finalize,_appendImage,_malloc,_free
    -sEXPORTED_RUNTIME_METHODS=FS 
    -sMODULARIZE 
    -sEXPORT_NAME=VideoWriter
    -sTOTAL_MEMORY=1GB
)


add_executable(out main.cpp x264writer.hpp)
target_include_directories(out PRIVATE libx264/include ${OpenCV_INCLUDE_DIRS})
target_link_libraries(out ${libx64_LIBRARIES} ${OpenCV_LIBRARIES})