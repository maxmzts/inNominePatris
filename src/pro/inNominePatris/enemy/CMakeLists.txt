#Especificar donde se encuentran los ficheros cpp a compilar
target_sources(libModulos PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/Enemy.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/EnemyBat.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/EnemyNecromancer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Boss1.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Boss2.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Projectile.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Boss1Minion.cpp"
    #"${CMAKE_CURRENT_SOURCE_DIR}/Boss3.cpp"
)

#Y donde se encuentran los .h
target_include_directories(libModulos PUBLIC 
"${CMAKE_CURRENT_SOURCE_DIR}"
)

