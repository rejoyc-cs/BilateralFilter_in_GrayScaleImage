cmake_minimum_required(VERSION 3.15)

project(bilateralFilter)
find_package(OpenCV REQUIRED )

add_executable( display main.cpp )
target_link_libraries(display ${OpenCV_LIBS})
