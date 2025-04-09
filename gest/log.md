# Log del trabajo realizado por día

## 20230108 FidelAznar @(1h) 
Se trabajó en tareas generales y se documentó lo realizado.
  
## 20250223 JorgePelegrín @(30min)
Añadí el sprite de la espada al main de la plantilla.
Hice que la espada siempre estuviera delante del sprite del jugador en las cuatro posiciones.

## 20250224 MáximoMartínez @(1.25h)
Seguí un [tutorial de YouTube](https://www.youtube.com/playlist?list=PL6xSOsbVA1eb_QqMTTcql_3PdOiE928up) para empezar a comprender SFML.
Creé una clase `Game` que encapsula el motor de juego y la aproveché para entender la gestión de librerías con CMake.
El código no compilaba cuando dejé de trabajar.

## 20250225 MáximoMartínez @(10min)
Revisé el `CMakeLists.txt` y añadí una referencia faltante a la librería.
Conseguí compilar el trabajo de ayer.
Añadí `.vscode/settings.json` al `.gitignore`.

## 20250226 MáximoMartínez @(1h)
Antes y durante la clase de prácticas, seguí el tutorial mencionado.
No hice commit ya que el trabajo estaba sin terminar y estuve estudiando para otra asignatura durante la semana.

## 20250226 JorgePelegrín @(2h, 30min)
Añadí la clase de espada para empezar a probar el ataque básico y la habilidad.
Creé una clase de personaje básica y una de enemigo para poder probar el daño de los ataques.
Se presentaron algunos errores de compilación y falta de integración de los sprites para la visualización.

## 20250228 JorgePelegrín @(1h)
El proyecto ya compila, el ataque básico y la habilidad funcionan.
Falta dibujar al enemigo y la espada, y probar si el ataque básico hace daño.
El principal problema es que el personaje no se mueve con las flechas, solo con el dash.

## 20250303 DavidGonzález @(1h, 15min)
Creé el backlog del equipo en GitHub para organizar las tareas.
Añadí mis tareas al "To Do".

## 20250303 EstrellaDomínguez @(1h)
Solucioné errores de `CMakeLists.txt`.
Creé la clase `Interface` que contiene el código del Menú y el HUD.
Añadí mis tareas al "To Do".

## 20250303 EstrellaDomínguez @(30min)
Empecé a implementar el menú.
Compila sin errores, pero no se ve en pantalla lo que esperaba.

## 20250303 MáximoMartínez @(30min)
Debería dejar de hacer tutoriales.

## 20250305 MáximoMartínez @(2h)
Durante prácticas he comenzado a programar un sistema de detección de áreas. Se compone de 2 clases abstractas que sirven de interfaz para cualquier implementación que se les quiera dar. 
  - DetectedArea: existe para ser detectado.
  - DetectingArea: escanea instancias de DetectedArea dentro de su área.
He hecho una prueba de este sistema en el main.

## 20250305 JorgePelegrín @(2h, 45min)
Implementé un sistema de items que mejoran las estadísticas del personaje.
Falta probar si el sistema funciona correctamente.
El personaje ya se mueve, realiza dash en todas las direcciones y ataca, aunque solo hacia la derecha y la izquierda.
*(1 hora despúes de los anteriores)*
El personaje ya ataca hacia todas las direcciones
Implementado el pickUp de item, probado con uno para aumentar la distancia del dash
Cambio un poco de la estructura del log para que se vea mejor estructurado. 

## 20250305 EstrellaDomínguez @(1h)
He conseguido que salga texto en el menú
Ya se puede seleccionar entre las opciones y se cierra si pulsas en "Salir"

## 20250305 EstrellaDomínguez @(45min)
He puesto imagen de fondo en el menú y mejorado la estética.
Ahora se controla con las teclas "W" y "S" en vez de "Up" y "Down".
Actualmente está puesta la ruta completa (/home/estrella...) para la imagen de fondo y la fuente porque no funcionaba con "resources/background".

## 20250305 DavidGonzález @(3h)
He implementado la clase Character que es el personaje principal.
He implementado el movimiento del personaje en 8 direcciones de manera fluida y con una buena velocidad ajustable.
He implementado parametros de aceleracion y deceleracion para el movimiento del personaje, que sirven para hacer un movimiento mas orgánico y se pueden emplear para agregar mecánicas en un fututo.
He ajustado el tamaño del personaje.

## 20250305 JorgePelegrín @(2h)
He añadido un nuevo arma, la lanza, junto a su ataque básico. Es el mismo que el de la espada, pero con un más rango.
He añadido también su habilidad, que deja un portal al que te puedes teletransportar. 
He reestructurado las clases de las armas, para que sea más flexible a la hora de añadir parámetros a los métodos del ataque y la habilidad.
He logrado que se pueda cambiar de arma al pulsar un botón, pudiendo combinar sus habilidades.

## 20250308 MáximoMartínez @(1h)
Seguí haciendo pruebas y ampliaciones a la detección de áreas para ampliar sus posibilidades de uso y refrescar punteros de C++ (no me basta con que me lo haga ChatGPT). Intenté hacer un experimento con un vector dinámico pero no pude terminarlo por lo que no subí los cambios.

## 20250310 MáximoMartínez @(1h)
Para terminar el sistema de áreas y he implementado un **singleton** (también a modo de práctica después de haberlo visto en teoría) porque me venía bien para gestionar una lista dinámica en las pruebas dicho sistema. 

## 20250311 EstrellaDomínguez @(1h)
He terminado el menú principal, con su tipografía y fondo.
He comenzado a implementar el HUD.

## 20250312 JorgePelegrín @(1h, 30min)
He añadido varios items, los he clasificado dependiendo del arma que mejoren, o si mejoran al propio personaje en sí(como la velocidad). Estos items aparecen uno aleatorio de cada arma. Falta añadir la lógica de la doble arma al personaje para que aparezcan solo items de las armas equipadas. También he implementado cooldown a las habilidades.

## 20250312 EstrellaDomínguez @(1h, 30min)
He conseguido que al darle a nueva partida se cree la partida y te redirija al propio juego.
Ahora el personaje tiene una barra de vida.
Aparece un rectángulo abajo a la izquierda que posteriormente mostrará las habilidades de las armas y otros aspectos del jugador.

## 20250312 MáximoMartínez @(2h, 45min)
He empezado a diseñar el **sistema de audio** intentando aprovechar para que funcione a modo de **fachada del módulo de audio de SFML**. He empezado por los sonidos individuales, todo lo que no sea música. Después de mucha pelea con GPT, he conseguido que suenen muchos sonidos simultáneamente.

## 20250313 DavidGonzález @(5h)
He implementado la camara en movimiento que sigue al personaje y un sistema de tiles, tilesets y tilemaps para montar y cargar mapas flexibles y personalizables. No he conseguido que dicho sistema funcione correctamente todavía.

## 2020317 MáximoMartínez @(1h 20min)
(1h 20min) Empezado y terminado módulo fachada ```MusicPlayer``` para la reproducción de música. Tiene unos métodos de fade y transición que necesitan ajustes, pero funciona.

## 20250317 JorgePelegrín @(30min)
He añadido la lógica para poder llevar hasta dos armas equipadas.

## 20250317 EstrellaDomínguez (1h, 15min)
He añadido la opción de Configuración en el menú principal.
He creado el menú Configuración (y su acceso desde el principal), así como sus diferentes opciones.

## 2020322 MáximoMartínez @(4h)
He mejorado el sistema de áreas para que permita distintias formas.
He creado ya una clase entidad, una clase hitbox y una clase hurtbox para implementar el "combate" o interacción del jugador con los enemigos.
He estado mucho tiempo y no he llegado a nada funcional porque una idea que tenía resultaba en una referencia circular y las IA no me avisaron. 

## 20250401 DavidGonzález @(12h)
He conseguido que el juego cargue mapas parseando archivos exportados desde la aplicacion tiled (carga el mapa. con sus capas aunque todavia no he conseguido que cargue distinos tilesets, por tanto no puedo emplear el tileset de la decoracion de momento). También he comenzado con el dibujado de mapas, ya están hechos la sala_1 que puede servir como tutorial, y parte del lobby. La realización de todo esto está siendo bastante tediosa y no hice commits hasta ahora ya que no obtenía progresos útiles, de ahí la gran cantidad de horas que he invertido para este commit.
IMPORTANTE: A partir de ahora para ejecutar el programa se debe instalar *Tinyxml12*, para ello escribimos los siguientes comandos en la plantilla del proyecto (Davinson en mi caso):
- sudo apt install pkg-config
- sudo apt install libtinyxml2-dev
- pkg-config --modversion tinyxml2 (para comprobar que se ha instalado correctamente)

## 20250402 JorgePelegrin @(1h 30min)
Arco y habilidad.

## 20250402 EstrellaDominguez @(1h 30min)
Creada la rama developer.
Arregladas las rutas para acceder a los recursos desde los menus.
Terminada mi parte del diagrama.

## 20250402 DavidGonzález @(3h)
He creado los limites del mapa mediante una capa bounds en tiled, y he cambiado algunos ajustes + sigo tileando el lobby

## 20250402 DavidGonzález @(2h 30 min)
Estoy realizando la interaccion (que todavia no funciona al 100%) y está el lobby terminado con los bounds, las puertas y los bloques de interaccion puestos

## 20250402 DavidGonzález, JorgePelegrin, EstrellaDominguez @(30 min)
Creacion de la rama developer correctamente y creación de la estrcutura de carpetas

## 20250402 DavidGonzález, JorgePelegrin, EstrellaDominguez, MáximoMartínez @(30 min)
Definición del diagrama de clases del proyecto.

## 20250402 MáximoMartínez @(1h)
He descartado absolutamente todo lo que he hecho hasta el momento y me he puesto a ver qué puedo hacer e investigar.

## 20250402 JorgePelegrin @(1h)
He creado varios objetos y he añadido el portal animado (la habilidad de la lanza).

## 20250405 JorgePelegrin @(4h)
He creado el motor de juego y el façade de la clase Sprite de sfml. Además he añadido las armas y he hecho un main "funcional". En el main se dibuja el mapa y el personaje junto a su arma equipada.

## 20250402 DavidGonzález @(2h)
Diseño de los niveles y el flujo del juego + comienzo de creacion del mapa world_1 + modificaciones en el workspace + intentos de modificar el motor para que corra al personaje (está en proceso)

## 20250406 JorgePelegrin @(1h)
Unificación de character de david y mio para el movimiento del character con arma, golpeo y dasheo

## 20250402 DavidGonzález @(2h)
Unificación de character de Jorge y mio para el movimiento del character con arma, golpeo y dasheo. Creado archivo notion con los ids de las interacciones de cada mapa y actualización del lobby

## 20250407 JorgePelegrin @(2h)
He colocado las armas dentro del lobby para poder equiparlas utilizando el boton "E". Falta restringir las armas para que se puedan ir equipando a medida que pasemos los mundos.

## 20250402 DavidGonzález @(2h)
He metido el mapa del mundo 1 que está en proceso. He introducido una funcion para que el jugador spawnee en la parte del mapa que se le indique

## 20250408 MáximoMartínez @(4h)
He empezado a diseñar los enemigos del videojuego creando una clase definitiva EnemyA (la "A" para no tener conflicto con la clase prototipo que no hemos eliminado todavía) con unas clases totalmente recodificadas de Hitbox y Hurtbox. Luego además he implementado como he podido el algoritmo A* para que vayan hacia el jugador. NADA DE ESTO ESTÁ TESTEADO a fecha de este log, por lo que he hecho commit pero no he pusheado por si las moscas, simplemente para dejar constancia.
## 20250407 EstrellaDomínguez @(4h)
He modificado todos los códigos para aplicar correctamente el patrón State.
Ahora hay una carpeta State con todos los estados y desde el main se controla el estado inicial.
Hay un par de fallos: La ventana del menú no se cierra cuando se abre el lobby, lo cuál creo que es lo que hace que no funcione correctamente el juego (el personaje no se mueve).

## 20250408 EstrellaDomínguez @(2h)
He intentado arreglar el error de las ventanas pero no he conseguido nada.
He cambiado el main para controlar mejor algunas excepciones y he cambiado el menú configuración para que se controle con WASD en vez de Up y Down.

## 20250408 JorgePelegrin @(1h 30min)
He arreglado el error de las dos ventanas, y ahora funciona el InGame perfectamente.

## 20250408 DavidGonzález @(6h)
Interacción implementada con éxito e incluida dentro del patrón state + modificaciones en world_1 que sigue en proceso
