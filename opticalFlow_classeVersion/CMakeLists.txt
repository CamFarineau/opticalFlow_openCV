cmake_minimum_required (VERSION 2.6)

include(CMakeToolsHelpers OPTIONAL)

set (CMAKE_CXX_STANDARD 11)

project (opticalFlow_classVersion)

find_package(OpenCV)
if(OpenCV_FOUND)
   message("Found OpenCV")
   message("Includes: " ${OpenCV_LIBS})
endif(OpenCV_FOUND)


find_package(Boost COMPONENTS filesystem)

file(GLOB_RECURSE opticalFlow_classVersion_SOURCES "src/*.cpp")
file(GLOB_RECURSE opticalFlow_classVersion_HEADERS "src/*.hpp")

set (opticalFlow_classVersion_INCLUDE_DIRS "")
foreach (_headerFile ${opticalFlow_classVersion_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND opticalFlow_classVersion_INCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES opticalFlow_classVersion_INCLUDE_DIRS)

add_executable(opticalFlow_classVersion ${opticalFlow_classVersion_SOURCES})
target_include_directories(opticalFlow_classVersion PRIVATE ${opticalFlow_classVersion_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
target_link_libraries(opticalFlow_classVersion ${OpenCV_LIBS} ${Boost_LIBRARIES})