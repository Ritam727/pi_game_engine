add_library(ecs STATIC)

file(GLOB_RECURSE ECS_SOURCES CONFIGURE_DEPENDS *.cpp)
file(GLOB_RECURSE ECS_HEADERS CONFIGURE_DEPENDS *.hpp)

target_sources(ecs
  PRIVATE
    ${ECS_SOURCES}
  PUBLIC
    FILE_SET HEADERS
    FILES
      ${ECS_HEADERS}
)

target_include_directories(ecs
  PUBLIC
    include
)

target_link_directories(ecs
  PUBLIC
    src
)