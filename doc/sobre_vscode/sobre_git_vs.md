# Antes de nada
Si no conoces nada de control de versiones ni de git echa un vistazo a: https://david-estevez.gitbooks.io/the-git-the-bad-and-the-ugly/content/es/usando-git.html


# Git con VSCODE
Antes de nada debe de estar configurado el usuario de git en vuestra máquina. Ejecutar en terminal:

~~~
git config --global user.email "you@example.com"
git config --global user.name "Your Name"
~~~

## Autentificación
Podeis realizar el control de versiones desde línea de comandos con git o bien con VSCODE. De todas maneras debéis conocer los comandos, ya que VSCODE se basa en ellos. 

**VSCODE os solicitará autentificarse en github la primera vez que hagáis uso de él.** Introducir vuestro usario y contraseña y autorizarlo:

![](/otros/img/pasog2.png)

El proceso de autorización abrirá una URL en el navegador donde pondremos nuestro usuario y contraseña de git. Es posible que os pida la contraseña del navegador para gestionar claves. Es la de vuestro usuario (`osboxes.org` en la máquina virtual).

Por último el navegador solicitará permiso para abrir VSCODE. Dárselo y ya estará conectado git con la extensión de git de VSCODE

## Uso básico de git desde VSCODE
Lo primero que debemos hacer es obtener la última versión del proyecto mediante un `git pull`. Eso lo debemos realizar desde la pestaña de git `ctrl+shift+G`, donde trabajaremos en esta sección. Pulsando en el botón superior izquierdo (...) podemos ejecutar el comando `pull`.

Ahora podemos realizar las tareas de desarrollo que consideremos. En la pestaña de git podemos crear ramas, poner etiquetas o hacer `commits` (parciales o totales) de nuestro trabajo. Los `commits` se hacen de manera muy intuitiva, introduciendo el texto que describe la tarea que hemos realizado y pulsando `ctrl+intro`. 

Como bien sabes, los commits se hace en local. Para subir los cambios al servido (haz este proceso cuando acabes una sesión de trabajo **si no hay constancia en el servidor de tu sincronización no puedo realizar la evaluación continua convenientemente y asumiré que no has desarrollado ninguna tarea**). Para ello debes:

* Verificar si hay cambios en el servidor git, y si es así, actualizar tu versión del repositorio, por ejemplo con `pull`
* Enviar tus cambios con `push` (se hace de la misma manera que como hiciste pull)

Todas las tareas de git más habituales se pueden hacer de esta manera. Ten en cuenta que además **existen plugins de vscode** que permiten por ejemplo de manera gráfica las ramas de un repositorio, y te pueden ser útiles (aunque no son necesarios y en un principio tal vez puedan liar si no tenéis experiencia con git).


## Uso de git desde línea de comandos
Tan simple como abrir una terminal en VSCODE (botón derecho en la carpeta raiz y abrir terminal) y desde ella ejecutar los comandos git que consideres. De todas manera recomiento usar VSCODE para gestionar git, por su sencillez y potencia.

# Un proyecto en tres ramas
El proyecto inicial de git que disponeis tiene tres ramas. Lo ideal es que se creen nuevas ramas por cada característica que se quiera implementar. Las nuevas ramas se integrarán a los pocos días, con el trabajo desarrollado, a su rama matriz.

Se debe respetar la estructura de ramas inicial de la siguiente manera:

* **[main]** (antiguamente se llamaba master en github): rama utilizada para versiones de entrega. Debemos asegurarnos que el dia de la entrega contiene todo lo que queremos que se corriga. **NO SE CORREGIRÁ NADA** que este fuera de esta rama. Esta rama, por tanto, no se actualiza muy a menudo.

* **[developer]**: rama donde se integran los prototipos ya con estructura. Es una rama que se actualiza con respecto a la evolución del proyecto, y que contiene una estructura de código razonable. Tambien debe ser la rama que contempla la documentación actualizada así como el resto de carpetas del proyecto (gestión...). 

* **[experimental]**: rama para hacer las pruebas iniciales, los prototipos... Se trata de una rama que se actualizará mucho (diariamente). En esta no es obligatorio que el código esté estructurado o que exista documentación. Es la rama para probar las cosas antes de integrarlas o reescribirlas en **developer**

* Podeis crear otrar ramas que consideréis **siempre y cuando estén asociadas a funcionalidades y no a personas**.

Te recomiendo que leas este documento de como organizar un proyecto software con git: https://david-estevez.gitbooks.io/the-git-the-bad-and-the-ugly/content/es/buenas-practicas-al-trabajar-con-git.html

