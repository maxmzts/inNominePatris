# Sobre esta plantilla

## Introducción
Esta carpeta (`proyecto`) contempla todo lo necesario para desarrollar el proyecto de la asignatura FV, con lo que os debéis familiarizar con ella cuanto antes. 

Se basa en el uso de herramientas libres, multiplataforma, para que podáis desarrollar cualquier proyecto con c++. Está configurada para poderse utilizar con VSCODE (recomendado) o desde línea de comandos.

La plantilla se debe obtener mediante el siguiente comando (¡¡¡no descargar en zip!!!): `git clone <url_de_tu_repositorio>`. De esa manera, en la carpeta `proyecto` tendremos todos los ficheros de la misma y toda la información del control de versiones:

![](/otros/img/paso1.png)

El comando anterior creará la carpeta `proyecto` en el escritorio, que contendrá la plantilla.

*Atención:* la url de la imagen anterior **NO** será la vuestra, debeís obtenerla como se indica en el frontal de vuestro repositorio en github.

## Estructura de carpetas
La estructura de carpetas la plantilla **no se puede modificar**, exceptuando el agregar más subproyectos dentro de la carpeta `src/pro`. Se pueden agregar los ficheros que se consideren necesarios con coherencia a la estructura:

* `.vscode`: ficheros de configuración de vscode.  Observa también el fichero del workspace (proyecto) `proyecto.code-workspace` que está en `/proyecto`. Abrelo y determina para que sirve. Se trata de ficheros de texto que es conveniente observar y os tocará modificar para adaptar el proyecto a vuestras necesidades.
* `.git`: información del control de versiones (no tocar)
* `doc`: documentación de vuestro proyecto en formato *markdown*. Se trata de un formato sencillo de marcado, usado por github y que utilizaremos para la práctica (si no lo conocéis dedicar 15' a leer sobre su uso). Solo se puede gastar este formato para documentar. Tal vez se solicite generar un pdf a partir de mismo (lo puede hacer VSCODE de manera sencilla).
* `eval`: ficheros markdown donde deberéis reflejar la autoevaluación de la práctica. Se indicará vuestro trabajo cuando corresponda
* `gest`: ficheros de organización y gestión del proyecto. Incluye las tareas, el reparto de trabajo y el log diario
* `otros`: otros ficheros y subcarpetas que consideréis

La parte de implementación se encuetra en la carpeta `src`. Esta carpeta contendrá una subcarpeta por prototipo a desarrollar. En la entrega final debe contener exclusivamente **en la rama principal** el prototipo final del juego.

## Cómo agregar nuevos subproyectos
El método más sencillo es el siguiente:

1. Hacer un `git pull` para asegurar que tenemos la última versión del proyecto
2. Duplicar la carpeta de la plantilla con el nombre que queramos (la copia debe estar dentro de la carpeta `src/pro`)
3. Agregar el nuevo subproyecto como workspace de VSCODE. Esto es requisito para poderlo compilar y ejecutar con facilidad. Para ello modificar el fichero `proyecto.code-workspace` y agregar el subproyecto como se indica en el mismo.
4. Ahora debéis ver el subproyecto en el árbol de directorios. En las siguientes secciones se indicará como compilar un proyecto.
5. Añadir la carpeta build del subproyecto en el fichero `.gitignore`, ya que no queréis control de versiones de los binarios.

## Compilar proyectos y subproyectos
La compilación se realiza mediante CMAKE->MAKE->GCC. CMake se encarga de generar un fichero make compatible con tu sistema que contenga las cuestiones relativas al proyecto. La información de como compilar el proyecto se encuentra concretamente en un fichero `CMakeLists.txt`. Por ejemplo, como compilar la plantilla se describe en `proyecto/src/pro/plantilla_fv/CMakeLists.txt`

**Atención:** Conforme añadas más ficheros fuente o librerias tendrás que editar el fichero `CMakeLists.txt` para que tenga constancia el CMake. No basta con agregarlos al proyecto.

### Mediante línea de comandos
1. Nos vamos a la carpeta del subproyecto a ejecutar, en nuestro caso la plantilla en `proyecto/src/pro/plantilla_fv`
2. Botón derecho encima y `Open in integrated terminal`. Eso abre un shell donde podemos trabajar cómodamente sin dejar el entorno
3. Ejecutar `./clean.sh` para que se elimine la carpeta `build`. Este paso es opcional y solo se debe hacer cuando empezamos la compilación después de sincronizar o bien cuando deseemos eliminar dicha carpeta, donde se encuentran los binarios del proyecto
4. Compilar con `./build.sh` 
5. Ejecutar el proyecto con `./build/nombre_binario`. Cuidado, es posible que el proyecto requiera estar en el directorio `./` por tema de rutas, por ejemplo de imágenes.
   
### Mediante VSCODE
1. Seleccionar el subproyecto activo. Como vuestro proyecto puede contener varios subproyecto se debe seleccionar el que se desea compilar. Para ello pulsar `ctrl+shift+P` para mostrar la consola de comandos. 
2. Introducir el comando `cmake select active folder` y ejecutarlo. Debeis seleccionar el proyecto a compilar. En nuestro caso vamos a compilar la plantilla, con lo que seleccionaremos `PROT: plantilla_fv`
3. Lo compilamos automáticamente mediante CMAKE mediante `ctrl+shift+P` y el comando `cmake build`
    * Puede solicitar la versión del compilador. Debéis poneros TODOS deacuerdo con que compilador usar. El más moderno es CLang, pero no hay problema si queréis gastar GCC. Eso si: ¡todos el mismo y la misma versión!
4. Lo ejecutamos en la pestalla run `ctrl+shift+D`, seleccionando la opción que corresponda (launch o debug) y pulsando sobre la flecha verde (o la tecla `F5`)

## Debug
Una vez compilado el proyecto vamos a la pantalla de run `ctrl+shift+D` y nos aseguramos que está selecionada la opción debug (gdb). Ahora podemos ir al fichero que queramos del proyecto (en el caso de la plantilla `main.cpp`) y pulsando en la parte izquierda de los números de línea agregamos los breakpoints deseados. Cuando ejecutemos el fichero (por ejemplo con `F5`) parará en la línea indicada y podremos ver el valor de las variables, ejecutar paso a paso...

### ¿Que pasa si me da este error?
![](/otros/img/err1.png)

Puede ocurrir por dos razones:
* Que no has compilado el proyecto. Ves a la sección anterior de como compilar un proyecto.
* Que el ejecutable que genera CMAKE no se llama como esperas. En ese caso verifica como se llama el ejecutable (cambia su nombre en CMakeLists.txt si lo consideras). Verifica también que tienes el nombre correcto en el launcher de VSCODE. Para ello edita el fichero `.vscode/launch.json` del subproyecto que corresponda (esto se puede hacer también desde la pestaña de ejecución de vscode dándole a la rueda dentada). Verifica que en la configuración `"program"` es lo que esperas.