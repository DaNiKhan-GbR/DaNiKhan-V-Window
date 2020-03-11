project(dnkvw CXX)

add_library(${PROJECT_NAME} SHARED
    src/dnkvwAPI.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/interface)

target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_THREAD_LIBS_INIT} ${OpenCV_LIBS})

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
    target_compile_definitions(${PROJECT_NAME} PRIVATE DNKVW_WIN_EXPORT)
endif()
