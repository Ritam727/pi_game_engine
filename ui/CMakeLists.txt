add_library(ui STATIC)

file(GLOB_RECURSE UI_SOURCES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE UI_HEADERS CONFIGURE_DEPENDS include/*.hpp)

target_sources(ui
  PRIVATE
    ${UI_SOURCES}
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
  PUBLIC
    FILE_SET HEADERS
    FILES
      imgui/imgui.h
      ${UI_HEADERS}
)

target_include_directories(ui
  PUBLIC
    include
    imgui
)

target_link_directories(ui
  PUBLIC
    src
)

target_link_libraries(ui
  PUBLIC
    core
    gl
)