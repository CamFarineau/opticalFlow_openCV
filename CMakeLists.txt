cmake_minimum_required (VERSION 2.6)



include(CMakeToolsHelpers OPTIONAL)

project (opticalFlow_openCV)

find_package(OpenCV)
if(OpenCV_FOUND)
   message("Found OpenCV")
   message("Includes: " ${OpenCV_LIBS})
endif(OpenCV_FOUND)

add_executable(opticalFlow_openCV ./src/opticalFlow.cpp)

target_link_libraries(opticalFlow_openCV ${OpenCV_LIBS})