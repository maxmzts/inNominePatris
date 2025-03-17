#Especificar donde se encuentran los ficheros cpp a compilar
target_sources(audio_facade PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/SoundPlayer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/MusicPlayer.cpp"
)

#Y donde se encuentran los .h
target_include_directories(audio_facade PUBLIC 
"${CMAKE_CURRENT_SOURCE_DIR}"
)