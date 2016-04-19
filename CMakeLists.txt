cmake_minimum_required(VERSION 2.8.7)
project(simpleNW)

# project settings
set(PROJECT_VERSION 0.1.0)

# OPTIONS
option(BUILD_NUMBER "Use a buildnumber" -1)
option(RELEASE "Build a release version" off)

# compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")

# set rpath
set(CMAKE_MACOSX_RPATH on)
set(CMAKE_SKIP_BUILD_RPATH  false)
set(CMAKE_BUILD_WITH_INSTALL_RPATH false) 
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH true)
set(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
if("${isSystemDir}" STREQUAL "-1")
   set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
endif("${isSystemDir}" STREQUAL "-1")

# libary requirements
set(Boost_USE_MULTITHREADED on)
set(Boost_DEBUG on)
find_package(Boost 1.51.0)
include_directories(${Boost_INCLUDE_DIRS})

# include headers
include_directories(include)

# compile src
add_subdirectory(src)

