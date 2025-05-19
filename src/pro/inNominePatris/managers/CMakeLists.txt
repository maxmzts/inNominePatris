#Especificar donde se encuentran los ficheros cpp a compilar
target_sources(libModulos PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/VFXManager.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/SFXManager.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/EnemyManager.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/MusicManager.cpp"   
    "${CMAKE_CURRENT_SOURCE_DIR}/ItemManager.cpp"
)

#Y donde se encuentran los .h
target_include_directories(libModulos PUBLIC 
"${CMAKE_CURRENT_SOURCE_DIR}"
)

