#Especificar donde se encuentran los ficheros cpp a compilar
target_sources(area_class PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/DetectedArea.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/DetectingArea.cpp"
)

#Y donde se encuentran los .h
target_include_directories(area_class PUBLIC 
"${CMAKE_CURRENT_SOURCE_DIR}"
)