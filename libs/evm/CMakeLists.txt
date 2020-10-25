project(evm)

add_library(${PROJECT_NAME} SHARED)

target_compile_definitions(${PROJECT_NAME} PUBLIC -DMAKE_DLL)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE ${OpenCV_INCLUDE_DIRS}
        )
set_target_properties( ${PROJECT_NAME}
        PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${RUNTIME_CONTEXT}"
        RUNTIME_OUTPUT_DIRECTORY "${RUNTIME_CONTEXT}"
        )
set(EVM_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/evm)

add_subdirectory(src)

target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCV_LIBS})

#SET(MAKE_DLL OFF CACHE BOOL "")