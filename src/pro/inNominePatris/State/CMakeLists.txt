# Especificar dónde se encuentran los ficheros .cpp a compilar
target_sources(libModulos PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/State.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/InGame.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/MainMenu.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/PauseMenu.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/ConfMenu.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/KoScreen.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/LobbyState.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/World1State.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/World2State.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/WorldState.h"

)

# Y dónde se encuentran los .h
target_include_directories(libModulos PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}"
)