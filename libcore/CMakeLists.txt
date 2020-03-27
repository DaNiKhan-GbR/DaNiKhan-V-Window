project(dnkvw CXX)

cmrc_add_resource_library(dnkvw_resources ALIAS dnkvw::rc NAMESPACE dnkvw_rc
    data/dnn.caffemodel
    data/dnn.prototxt
    data/haarcascade_frontalface_default.xml
)

set(DNKVW_EXTRA_SOURCES )
set(DNKVW_COMPANY DaNiKhan)
set(DNKVW_DESCRIPTION "DaNiKhan Virtual Window Library")
set(DNKVW_PRODUCTNAME "DaNiKhan V-Window")
set(DNKVW_COPYRIGHT "(C) 2020, Daniel Klintworth and Nicolas Hollmann")
set(DNKVW_FILENAME dnkvw.dll)

if(MSVC)
    configure_file(
        ${PROJECT_SOURCE_DIR}/version.rc.in
        ${CMAKE_BINARY_DIR}/_winrc/version.rc
        @ONLY
    )
    list(APPEND DNKVW_EXTRA_SOURCES ${CMAKE_BINARY_DIR}/_winrc/version.rc)
endif()

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

    ${DNKVW_EXTRA_SOURCES}
)

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


target_include_directories(${PROJECT_NAME} PRIVATE ${OpenCV_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/include")
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/interface")

target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_THREAD_LIBS_INIT} ${OpenCV_LIBS} dnkvw::rc)

#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")

target_compile_definitions(${PROJECT_NAME} PRIVATE DNKVW_WIN_EXPORT)
