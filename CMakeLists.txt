cmake_minimum_required(VERSION 3.23)
project(engine)

add_compile_definitions(ENGINE_PATH="${CMAKE_BINARY_DIR}")

add_subdirectory(app)
add_subdirectory(gl)
add_subdirectory(glfw)
add_subdirectory(glad)
add_subdirectory(spdlog)
add_subdirectory(glm)
add_subdirectory(stb_image)
add_subdirectory(core)
add_subdirectory(json)
add_subdirectory(inputs)
add_subdirectory(assimp)

add_executable(engine main.cpp)
target_link_libraries(engine app)

file(COPY "res" DESTINATION "${CMAKE_BINARY_DIR}")

if (PROJECT_IS_TOP_LEVEL AND UNIX)
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E create_symlink
            ${CMAKE_BINARY_DIR}/compile_commands.json
            ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
    )
endif()