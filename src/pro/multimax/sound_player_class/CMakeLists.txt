#Especificar donde se encuentran los ficheros cpp a compilar
target_sources(sound_player_class PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/SoundPlayer.cpp"
)

#Y donde se encuentran los .h
target_include_directories(sound_player_class PUBLIC 
"${CMAKE_CURRENT_SOURCE_DIR}"
)