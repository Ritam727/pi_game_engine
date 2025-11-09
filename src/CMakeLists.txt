file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cpp)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS *.hpp)

target_sources(engine
  PRIVATE
    ${SOURCES}
    ${HEADERS}
)

target_link_libraries(engine
  PRIVATE
    # glad
    gl
    # glfw
    dl
    pthread
    GL
    X11
    Xrandr
)