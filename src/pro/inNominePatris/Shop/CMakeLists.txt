# Especificar dónde se encuentran los ficheros .cpp a compilar
target_sources(libModulos PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/Shop.cpp"
)

# Y dónde se encuentran los .h
target_include_directories(libModulos PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}"
)