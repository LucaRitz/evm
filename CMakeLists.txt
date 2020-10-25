cmake_minimum_required(VERSION 3.15)

project(evm_exe)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

add_subdirectory(ext)

set(BUILD_SHARED_LIBS ON)
find_package(OpenCV REQUIRED CONFIG
        PATHS ${CMAKE_BINARY_DIR}/ext/opencv)
unset(BUILD_SHARED_LIBS)

set(CMAKE_HELP_DIR ${CMAKE_SOURCE_DIR}/cmake)
set(RUNTIME_CONTEXT ${CMAKE_BINARY_DIR})

add_executable(${PROJECT_NAME})
target_include_directories(${PROJECT_NAME}
        PRIVATE ${OpenCV_INCLUDE_DIRS}
        )
file(COPY resources DESTINATION ${CMAKE_BINARY_DIR})

add_subdirectory(libs)
add_subdirectory(src)

target_link_libraries(${PROJECT_NAME} PRIVATE evm ${OpenCV_LIBS})

configure_file(${CMAKE_SOURCE_DIR}/cmake/copy_libraries.cmake ${CMAKE_BINARY_DIR}/copy_libraries_gen.cmake @ONLY)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/copy_libraries_gen.cmake)