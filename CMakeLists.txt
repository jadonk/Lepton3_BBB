cmake_minimum_required (VERSION 2.8.9)

project (lepton3_grabber)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
set(CMAKE_DEBUG_POSTFIX "_d")

#########################################################
# FLAGS
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_POSITION_INDEPENDENT_CODE ON)
#########################################################

# Library source code
add_subdirectory (grabber_lib)

# GStreamer demo
add_subdirectory (opencv_demo)
