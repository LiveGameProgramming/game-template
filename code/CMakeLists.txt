#=======================================================================================================================
add_executable(game)
#=======================================================================================================================
target_link_libraries(game PRIVATE game_engine module_platform module_graphics module_math module_core)
target_link_libraries(game PRIVATE game_editor)
       target_sources(game PRIVATE main.cpp)
#=======================================================================================================================
set_target_properties(game PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/build"
                                      RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/build")
#=======================================================================================================================