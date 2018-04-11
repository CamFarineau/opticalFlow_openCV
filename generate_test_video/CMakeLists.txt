cmake_minimum_required (VERSION 2.6)

include(CMakeToolsHelpers OPTIONAL)

project (generate_test_video)

find_package(OpenCV)
if(OpenCV_FOUND)
   message("Found OpenCV")
   message("Includes: " ${OpenCV_LIBS})
endif(OpenCV_FOUND)

add_executable(generate_test_video ./src/main.cpp)

target_link_libraries(generate_test_video ${OpenCV_LIBS})