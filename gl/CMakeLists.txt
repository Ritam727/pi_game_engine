add_library(gl STATIC)

file(GLOB_RECURSE GL_SOURCES CONFIGURE_DEPENDS *.c*)
file(GLOB_RECURSE GL_HEADERS CONFIGURE_DEPENDS *.hpp)

target_sources(gl
  PRIVATE
    ${GL_SOURCES}
  PUBLIC
    FILE_SET HEADERS
    FILES
      ${GL_HEADERS}
)

target_include_directories(gl
  PUBLIC
    include
)

target_link_directories(gl
  PUBLIC
    src
)

target_link_libraries(gl
  PUBLIC
    core
    
    glfw
    glad
    glm
    stb_image
)