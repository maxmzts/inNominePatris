# Especificar dónde se encuentran los ficheros .cpp a compilar
target_sources(libModulos PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/State.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/InGame.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/MainMenu.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/PauseMenu.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/ConfMenu.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/KoScreen.cpp"
)

# Y dónde se encuentran los .h
target_include_directories(libModulos PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}"
)