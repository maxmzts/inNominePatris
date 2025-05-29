#Especificar donde se encuentran los ficheros cpp a compilar
target_sources(libModulos PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/GameEngine.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/SpriteFacade.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/AnimatedSprite.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/VisualEffect.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/SoundEffect.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/MusicTrack.cpp"
)

#Y donde se encuentran los .h
target_include_directories(libModulos PUBLIC 
"${CMAKE_CURRENT_SOURCE_DIR}"
)

