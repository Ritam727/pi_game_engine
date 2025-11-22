add_library(core STATIC)

file(GLOB_RECURSE CORE_SOURCES CONFIGURE_DEPENDS *.c*)
file(GLOB_RECURSE CORE_HEADERS CONFIGURE_DEPENDS *.hpp)

target_sources(core
  PRIVATE
    ${CORE_SOURCES}
  PUBLIC
    FILE_SET HEADERS
    FILES
      ${CORE_HEADERS}
)

target_include_directories(core
  PUBLIC
    include
)

target_link_directories(core
  PUBLIC
    src
)

target_link_libraries(core
  PUBLIC
    spdlog
    glm
    glad
    glfw
    assimp
)