cmake_minimum_required(VERSION 3.1)
project(MiJuego)

#Añadir tantos subdirectorios como se necesiten
#En cada subdirectorio debe haber un fichero CMakeLists.txt
#Habitualmente cada subdirectorio determinará una libreria 
add_library(libModulos STATIC)
add_subdirectory(ej_modulos)
add_library(game_class STATIC)
add_subdirectory(game_class)
add_library(area_class STATIC)
add_subdirectory(area_class)
add_library(sound_player_class STATIC)
add_subdirectory(sound_player_class)

#Opciones a pasar al compilador de C++
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall -Wextra")
#Se desea generar un ejecutable, se especifica donde está el main
#y ficheros que aporten código que no sean librerias o subcarpetas 
add_executable(MiJuego main.cpp)

#Copio el ejecutable fuera de build para tener acceso fácil a los recursos
add_custom_command(TARGET MiJuego 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:MiJuego> ..)

#Añadir todos los objetos (ficheros o) que requiera el ejecutable. 
#Habitualmente todas las librerias definidas mediante subcarpetas
#aportaran código
target_link_libraries(MiJuego libModulos game_class area_class sound_player_class)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")
#Para usar SFML
find_package(SFML REQUIRED system window graphics network audio)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(MiJuego ${SFML_LIBRARIES})
endif()


