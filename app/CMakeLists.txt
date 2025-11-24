add_library(app STATIC)

file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS *.c*)
file(GLOB_RECURSE APP_HEADERS CONFIGURE_DEPENDS *.hpp)

target_sources(app
  PRIVATE
    ${APP_SOURCES}
  PUBLIC
    FILE_SET HEADERS
    FILES
      ${APP_HEADERS}
)

target_include_directories(app
  PUBLIC
    include
)

target_link_directories(app
  PUBLIC
    src
)


target_link_libraries(app
  PUBLIC
    core
    gl
    inputs
    ui
    
    dl
    pthread
    GL
    X11
    Xrandr
)