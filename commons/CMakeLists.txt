add_library(commons STATIC)

file(GLOB_RECURSE COMMONS_SOURCES CONFIGURE_DEPENDS *.cpp)
file(GLOB_RECURSE COMMONS_HEADERS CONFIGURE_DEPENDS *.hpp)

target_sources(commons
  PRIVATE
    ${COMMONS_SOURCES}
  PUBLIC
    FILE_SET HEADERS
    FILES
      ${COMMONS_HEADERS}
)

target_include_directories(commons
  PUBLIC
    include
)

target_link_directories(commons
  PUBLIC
    src
)