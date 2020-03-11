project(dnkvwtool CXX)

add_executable(${PROJECT_NAME} WIN32
    src/devtool.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} PRIVATE dnkvw)
