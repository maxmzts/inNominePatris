#Especificar donde se encuentran los ficheros cpp a compilar
target_sources(libModulos PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/Interaction.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/InteractionManager.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/InteractionFactory.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/ButtonInteraction.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/DoorInteraction.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/UiTriggerInteraction.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/SpawnPlayerInteraction.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/WorldChangeInteraction.cpp"
)

#Y donde se encuentran los .h
target_include_directories(libModulos PUBLIC 
"${CMAKE_CURRENT_SOURCE_DIR}"
)

