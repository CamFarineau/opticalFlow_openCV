cmake_minimum_required (VERSION 2.6)

include(CMakeToolsHelpers OPTIONAL)

set (CMAKE_CXX_STANDARD 11)

project (opticalFlow_GUI)

find_package(OpenCV)
if(OpenCV_FOUND)
   message("Found OpenCV")
   message("Includes: " ${OpenCV_LIBS})
endif(OpenCV_FOUND)


find_package(Boost COMPONENTS filesystem)
find_package(X11 REQUIRED)
find_package(ALSA REQUIRED)

file(GLOB_RECURSE opticalFlow_GUI_SOURCES "src/*.cpp")
file(GLOB_RECURSE opticalFlow_GUI_HEADERS "src/*.hpp")

set (opticalFlow_GUI_INCLUDE_DIRS "")
foreach (_headerFile ${opticalFlow_GUI_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND opticalFlow_GUI_INCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES opticalFlow_GUI_INCLUDE_DIRS)

add_executable(opticalFlow_GUI ${opticalFlow_GUI_SOURCES})
target_include_directories(opticalFlow_GUI PRIVATE ${opticalFlow_GUI_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} ${X11_INCLUDE_DIR})
target_link_libraries(opticalFlow_GUI ${OpenCV_LIBS} ${Boost_LIBRARIES} nana ${X11_LIBRARIES} rt ${X11_Xft_LIB} ${ALSA_LIBRARIES} fontconfig stdc++fs)