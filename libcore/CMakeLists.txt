project(dnkvw CXX)

add_library(${PROJECT_NAME} SHARED
    src/dnkvw.cpp
    src/dnkvwContext.cpp
    include/dnkvwContext.hpp
    include/constants.hpp
    include/tracker.hpp
    src/haarTracker.cpp
    include/haarTracker.hpp
    src/dnnTracker.cpp
    include/dnnTracker.hpp
    src/fpsTimer.cpp
    include/fpsTimer.hpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/interface)

target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_THREAD_LIBS_INIT} ${OpenCV_LIBS})
target_compile_definitions(${PROJECT_NAME} PRIVATE DNKVW_WIN_EXPORT)
