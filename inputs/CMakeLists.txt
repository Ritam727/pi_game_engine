add_library(inputs STATIC)

file(GLOB_RECURSE INPUTS_SOURCES CONFIGURE_DEPENDS *.c*)
file(GLOB_RECURSE INPUTS_HEADERS CONFIGURE_DEPENDS *.hpp)

list(FILTER INPUTS_HEADERS EXCLUDE REGEX "constants.hpp$")

target_sources(inputs
  PRIVATE
    ${INPUTS_SOURCES}
  PUBLIC
    FILE_SET HEADERS
    FILES
      ${INPUTS_HEADERS}
)

target_include_directories(inputs
  PUBLIC
    include
)

target_link_directories(inputs
  PUBLIC
    src
)

target_link_libraries(inputs
  PUBLIC
    nlohmann_json
    core
    glfw
)