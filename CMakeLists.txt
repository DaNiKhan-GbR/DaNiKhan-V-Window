###############################################################################
# DaNiKhan Virtual Window (dnkvw) CMake file                                  #
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

project("DaNiKhan VWindow" LANGUAGES CXX VERSION 1.0.0)

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
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# We need C++17 for dnkvw.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable position independent code on all targets
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Reconfigure the output directoy.
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Set a postfix for debug builds.
set(CMAKE_DEBUG_POSTFIX "d")

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

# Begin of RPath Helper

# Always full RPATH (for shared libraries)
#  https://gitlab.kitware.com/cmake/community/-/wikis/doc/cmake/RPATH-handling

# use the full RPATH for the build tree
set(CMAKE_SKIP_BUILD_RPATH FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# the RPATH to be used when installing, but only if it's not a system directory
list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
if("${isSystemDir}" STREQUAL "-1")
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
endif()

# End of RPATH helper.

# Add the project subdirectories
add_subdirectory(libcore)
add_subdirectory(devtool)

# Set the devtool as the start project if Visual Studio is used.
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT dnkvwtool)

# Output a summary.
feature_summary(WHAT ALL DESCRIPTION "Configure Summary:")

# END OF FILE
