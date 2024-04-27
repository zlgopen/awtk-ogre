cmake_minimum_required (VERSION 3.10)
project(AwtkOgre)

set(CMAKE_CXX_STANDARD 20)

find_package(OGRE REQUIRED COMPONENTS Bites CONFIG)
find_package(awtk REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/src)

file(GLOB LIB_SOURCES src/*.c src/*.cpp)
add_library(awtk_ogre ${LIB_SOURCES})
target_link_libraries(awtk_ogre OgreBites awtk)

add_executable(demo demos/main.cpp)
target_link_libraries(demo awtk_ogre OgreBites awtk)

install(TARGETS awtk_ogre
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

install(TARGETS demo DESTINATION bin)
