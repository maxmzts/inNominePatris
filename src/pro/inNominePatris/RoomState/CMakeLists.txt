#Especificar donde se encuentran los ficheros cpp a compilar
target_sources(libModulos PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/RoomManager.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/RoomState.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/DungeonRoom.cpp"
    #"${CMAKE_CURRENT_SOURCE_DIR}/BossRoom.cpp"
)

#Y donde se encuentran los .h
target_include_directories(libModulos PUBLIC 
"${CMAKE_CURRENT_SOURCE_DIR}"
)

