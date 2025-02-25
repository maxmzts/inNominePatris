# Instalación
Este proyecto requiere la instalación de las siguientes librerias/aplicaciones

## Linux

* sudo apt-get install libsfml-dev
* sudo apt-get install cmake
* sudo apt-get install clang

+ VSCode. Dentro de el mismo se requieren los siguientes plugins. Estos se buscan en el apartado de plugins de la barra de la izquierda (la L formada de 3 cajas)
    * @id:ms-vscode.cpptools+
    * @id:vector-of-bool.cmake-tools

# CMAKE
Para incluir SFML en el proyecto, propongo la siguiente plantilla. Esta depende de si SFML es >= 2.5 o no. Aquí adjunto las dos versiones (la versión de 2.4 también va en 2.5 por ahora)


## cmake sfml 2.5

    cmake_minimum_required(VERSION 2.8)

    project(prueba)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall -Wextra")


    find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
    add_executable(prueba main.cpp)
    target_link_libraries(prueba sfml-graphics sfml-audio)


## cmake sfml 2.4

    cmake_minimum_required(VERSION 2.8)

    project(prueba)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall -Wextra")

    add_executable(prueba main.cpp)

    set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")
    find_package(SFML REQUIRED system window graphics network audio)
    if (SFML_FOUND)
        include_directories(${SFML_INCLUDE_DIR})
        target_link_libraries(prueba ${SFML_LIBRARIES})
    endif()

