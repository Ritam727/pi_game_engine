add_library(stb_image STATIC)

file(GLOB_RECURSE STB_IMAGE_SOURCES CONFIGURE_DEPENDS *.cpp)
file(GLOB_RECURSE STB_IMAGE_HEADERS CONFIGURE_DEPENDS *.hpp)

target_sources(stb_image
  PRIVATE
    ${STB_IMAGE_SOURCES}
  PUBLIC
    FILE_SET HEADERS
    FILES
      ${STB_IMAGE_HEADERS}
)

target_include_directories(stb_image
  PUBLIC
    include
)

target_link_directories(stb_image
  PUBLIC
    src
)