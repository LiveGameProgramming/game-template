#=======================================================================================================================
project(game)
#=======================================================================================================================
add_executable(${PROJECT_NAME})
#=======================================================================================================================
target_link_libraries(${PROJECT_NAME} PRIVATE assets_module platform_module graphics_module math_module core_module)
target_link_libraries(${PROJECT_NAME} PRIVATE editor_core)
target_link_libraries(${PROJECT_NAME} PRIVATE engine_core)
target_sources(${PROJECT_NAME}        PRIVATE main.cpp)
#=======================================================================================================================
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/build"
                                                 RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/build")
#=======================================================================================================================