file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.c*)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS *.hpp)

target_sources(pi_game_engine
  PRIVATE
    ${SOURCES}
  PUBLIC
    FILE_SET HEADERS
    FILES
      ${HEADERS}
)

target_link_libraries(pi_game_engine
  PUBLIC
    core
    app

    nlohmann_json
)