cmake_minimum_required (VERSION 3.5)

# Disable in-source builds to prevent source tree corruption. (From OpenCV Makefile)
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR "
FATAL: In-source builds are not allowed.
       You should create a separate directory for build files.
")
endif()

project("DaNiKhan VWindow" CXX)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message("Setting build type to 'Release' as none was specified.")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
include(CMakeRC)
include(FeatureSummary)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

find_package(Threads REQUIRED)
set_package_properties(Threads PROPERTIES
    TYPE REQUIRED
    PURPOSE "Required for background calculation of face position."
)

find_package(OpenCV 4.2 REQUIRED)
set_package_properties(OpenCV PROPERTIES
    URL "https://opencv.org/"
    TYPE REQUIRED
    PURPOSE "Required for all image processing and video capturing tasks."
)

add_subdirectory(libcore)
add_subdirectory(devtool)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT dnkvwtool)

feature_summary(WHAT ALL DESCRIPTION "Configure Summary:")
