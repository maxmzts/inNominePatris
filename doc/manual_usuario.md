# Manual de usuario

## Requisitos previos 
- Ubuntu o cualquier distribución Linunx compatible.
- Tener cmake y g++ (o similar) instalados.
- Conexión a internet (solo la primera vez, para descargar los recursos gráficos).

## Instrucciones de ejecución
1. Desde la carpeta raíz del proyeto, abre una termianal y navega a: ```cd src/pro/inNominePatris```
2. Limpieza del entorno previo: ```./clean.sh```
3. Descarga de los recursos gráficos y otros assets: ```./setup.sh```
4. Preparación del entorno de compilación: ```./build.sh```
5. Compilación del proyecto: ```cmake --build build```
6. Ejecución del juego: ```./MiJuego```

## Notas adicionales
- setup.sh solo necesita ejecutarse una vez, a no ser que se b orren los assets o se actualicen los enlaces de descarga.
- Si alguno de los scripts no se ejecuta, verifica que tienen permisos con: ```chmod +x nombre.sh```
- Todos los scripts deben ejecutarse desde la carpeta ```src/pro/inNominePatris```, no desde la raíz del repositorio.

