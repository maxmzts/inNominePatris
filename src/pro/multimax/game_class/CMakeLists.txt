#Especificar donde se encuentran los ficheros cpp a compilar
target_sources(game_class PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/Game.cpp"
)

#Y donde se encuentran los .h
target_include_directories(game_class PUBLIC 
"${CMAKE_CURRENT_SOURCE_DIR}"
)