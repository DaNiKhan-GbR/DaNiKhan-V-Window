###############################################################################
# DaNiKhan Virtual Window (dnkvw) Cmake file                                  #
#                                                                             #
# This is the main cmake file and the enty point for compilation.             #
#                                                                             #
# In-Source builds are explicitly disabled!                                   #
# Please refere to the README.md for build instructions.                      #
###############################################################################

cmake_minimum_required (VERSION 3.5)

# Disable in-source builds to prevent source tree corruption. (From OpenCV Makefile)
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR "
FATAL: In-source builds are not allowed.
       You should create a separate directory for build files.
")
endif()

project("DaNiKhan VWindow" CXX)

# Default to Release if no build type was specified.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message("Setting build type to 'Release' as none was specified.")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Set module path and include required modules.
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
include(CMakeRC)
include(FeatureSummary)

# We need C++17 for dnkvw.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Reconfigure the output directoy.
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Find Threads package.
find_package(Threads REQUIRED)
set_package_properties(Threads PROPERTIES
    TYPE REQUIRED
    PURPOSE "Required for background calculation of face position."
)

# Find OpenCV 4.2 package.
find_package(OpenCV 4.2 REQUIRED)
set_package_properties(OpenCV PROPERTIES
    URL "https://opencv.org/"
    TYPE REQUIRED
    PURPOSE "Required for all image processing and video capturing tasks."
)

# Add the project subdirectories
add_subdirectory(libcore)
add_subdirectory(devtool)

# Set the devtool as the start project if Visual Studio is used.
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT dnkvwtool)

# Output a summary.
feature_summary(WHAT ALL DESCRIPTION "Configure Summary:")

# END OF FILE
