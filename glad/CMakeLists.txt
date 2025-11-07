add_library(glad STATIC)

file(GLOB_RECURSE GLAD_SOURCES CONFIGURE_DEPENDS *.c*)
file(GLOB_RECURSE GLAD_HEADERS CONFIGURE_DEPENDS *.hpp)

target_sources(glad
  PRIVATE
    ${GLAD_SOURCES}
  PUBLIC
    FILE_SET HEADERS
    FILES
      ${GLAD_HEADERS}
)

target_include_directories(glad
  PUBLIC
    include
)

target_link_directories(glad
  PUBLIC
    src
)