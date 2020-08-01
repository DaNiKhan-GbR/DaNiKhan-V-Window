###############################################################################
# DaNiKhan Virtual Window (dnkvw) devtool Cmake file                          #
#                                                                             #
# This is a sub cmake file for the dnkvw library.                             #
# DO NOT USE THIS DIRECTLY AS A CMAKE PROJECT.                                #
# ONLY USE THE MAIN CMakeLists.txt!                                           #
#                                                                             #
# In-Source builds are explicitly disabled!                                   #
# Please refere to the README.md for build instructions.                      #
###############################################################################

project(dnkvw 
    LANGUAGES CXX 
    VERSION 1.0.0
)

# Add the required resources to the binary.
cmrc_add_resource_library(dnkvw_resources ALIAS dnkvw::rc NAMESPACE dnkvw_rc
    data/dnn.caffemodel
    data/dnn.prototxt
    data/haarcascade_frontalface_default.xml
)

# Set dnkvw meta informations. 
set(DNKVW_EXTRA_SOURCES )
set(DNKVW_COMPANY DaNiKhan)
set(DNKVW_DESCRIPTION "DaNiKhan Virtual Window Library")
set(DNKVW_PRODUCTNAME "DaNiKhan V-Window")
set(DNKVW_COPYRIGHT "(C) 2020, Daniel Klintworth and Nicolas Hollmann")
set(DNKVW_FILENAME dnkvw.dll) # Windows specific

# Only add resource file if MSVC is used.
if(MSVC)
    configure_file(
        ${PROJECT_SOURCE_DIR}/version.rc.in
        ${CMAKE_BINARY_DIR}/_winrc/version.rc
        @ONLY
    )
    list(APPEND DNKVW_EXTRA_SOURCES ${CMAKE_BINARY_DIR}/_winrc/version.rc)
endif()

# The library itself.
add_library(${PROJECT_NAME} SHARED
    src/dnkvw.cpp
    src/dnkvwContext.cpp
    include/dnkvwContext.hpp
    include/resources.hpp
    include/constants.hpp
    include/tracker.hpp
    src/haarTracker.cpp
    include/haarTracker.hpp
    src/dnnTracker.cpp
    include/dnnTracker.hpp
    src/thrdWinCalc.cpp
    include/thrdWinCalc.hpp
    src/fpsTimer.cpp
    include/fpsTimer.hpp
    src/vec3.cpp
    include/vec3.hpp
    src/logger.cpp
    include/logger.hpp

    ${DNKVW_EXTRA_SOURCES}
)

# The public interface.
set(PUBLIC_INTERFACE
    interface/dnkvw/dnkvw.h
    interface/dnkvw/dnkvwInterface.h
)
set(PUBLIC_INTERFACE_DIR "${PROJECT_SOURCE_DIR}/interface")

# Output a warning if the shared version of OpenCV is used.
# The static variant is preferred because the final binary
# is self included without further dependencies.
if(OpenCV_SHARED)
    message(WARNING " The selected OpenCV version is built as a shared library.\n"
                    " You can use it, but you always need to deploy the\n"
                    " OpenCV shared libraries with your applications.\n\n"
                    " The recommended way is to build OpenCV as a static library,\n"
                    " then change the OpenCV_DIR variable for this project to the\n"
                    " static OpenCV install dir. This way dnkvw includes all needed\n"
                    " functions and can be used without other dependencies.\n"
    )
endif()

add_feature_info("Static OpenCV" "NOT ${OpenCV_SHARED}" "linking OpenCV as a static library.")

# Add all needed include directories.
target_include_directories(${PROJECT_NAME} PRIVATE ${OpenCV_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/include")
target_include_directories(${PROJECT_NAME} PUBLIC 
   "$<BUILD_INTERFACE:${PUBLIC_INTERFACE_DIR}>"
   "$<INSTALL_INTERFACE:.>"
)

# Link with all required libraries.
target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_THREAD_LIBS_INIT} ${OpenCV_LIBS} dnkvw::rc)

# Note: Its required that the /MT /MD /MTd /MDd setting is the same for all
#       software components in an Windows application.
#       Maybe an cmake option for this would be nice.
# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")

# Set the export define.
target_compile_definitions(${PROJECT_NAME} PRIVATE DNKVW_WIN_EXPORT)

# Configure paths.
set(CONFIG_INSTALL_DIR  "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
set(GENERATED_DIR       "${CMAKE_BINARY_DIR}/generated")
set(VERSION_CONFIG_FILE "${GENERATED_DIR}/${PROJECT_NAME}ConfigVersion.cmake")
set(PROJECT_CONFIG_FILE "${GENERATED_DIR}/${PROJECT_NAME}Config.cmake")
set(TARGETS_EXPORT_NAME "${PROJECT_NAME}Targets")

# Configure '<PROJECT-NAME>ConfigVersion.cmake'
write_basic_package_version_file(
    "${VERSION_CONFIG_FILE}"
    VERSION "${${PROJECT_NAME}_VERSION}"
    COMPATIBILITY SameMajorVersion
)

# Configure '<PROJECT-NAME>Config.cmake'
configure_package_config_file(
    "${CMAKE_SOURCE_DIR}/cmake/Config.cmake.in"
    "${PROJECT_CONFIG_FILE}"
    INSTALL_DESTINATION "${CONFIG_INSTALL_DIR}"
)

# Uninstall targets
configure_file(
    "${CMAKE_SOURCE_DIR}/cmake/Uninstall.cmake.in"
    "${GENERATED_DIR}/Uninstall.cmake"
    IMMEDIATE @ONLY
)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${GENERATED_DIR}/Uninstall.cmake
)

# Install config.
install(
    TARGETS                 "${PROJECT_NAME}"
    EXPORT                  "${TARGETS_EXPORT_NAME}"
    LIBRARY DESTINATION     "${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION     "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION     "${CMAKE_INSTALL_BINDIR}"
    INCLUDES DESTINATION    "${CMAKE_INSTALL_INCLUDEDIR}"
)

# Install public interface
install(
    FILES ${PUBLIC_INTERFACE}
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}"
)

# Install Cmake Config
#   - <prefix>/lib/cmake/dnkvw/dnkvwConfig.cmake
#   - <prefix>/lib/cmake/dnkvw/dnkvwConfigVersion.cmake
install(
    FILES       "${PROJECT_CONFIG_FILE}"
                "${VERSION_CONFIG_FILE}"
    DESTINATION "${CONFIG_INSTALL_DIR}"
)

# Install Cmake Target Config
#   - <prefix>/lib/cmake/dnkvw/dnkvwTargets.cmake
install(
  EXPORT      "${TARGETS_EXPORT_NAME}"
  FILE        "${PROJECT_NAME}Targets.cmake"
  DESTINATION "${CONFIG_INSTALL_DIR}"
)

# Support for cmake registry.
include(CMakeRegistry)

# END OF FILE
