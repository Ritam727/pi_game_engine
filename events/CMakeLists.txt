add_library(events STATIC)

file(GLOB_RECURSE EVENTS_SOURCES CONFIGURE_DEPENDS *.cpp)
file(GLOB_RECURSE EVENTS_HEADERS CONFIGURE_DEPENDS *.hpp)

target_sources(events
  PRIVATE
    ${EVENTS_SOURCES}
  PUBLIC
    FILE_SET HEADERS
    FILES
      ${EVENTS_HEADERS}
)

target_include_directories(events
  PUBLIC
    include
)

target_link_directories(events
  PUBLIC
    src
)