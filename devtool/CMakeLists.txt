project(dnkvwtool CXX)

add_executable(${PROJECT_NAME}
    src/devtool.cpp
)

target_link_libraries(${PROJECT_NAME} dnkvw ${OpenCV_LIBS})
