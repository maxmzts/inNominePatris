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
He arreglado el error de las dos ventanas, y ahora funciona el ```InGame``` perfectamente.

## 20250408 DavidGonzález @(6h)
Interacción implementada con éxito e incluida dentro del patrón state + modificaciones en world_1 que sigue en proceso

## 20250402 DavidGonzález @(3h 30 min)
Imlementación de funcion de cambio de tiles de capa in game (para obtener input visuales y cambiar mapa in game).

## 20250408 EstrellaDomínguez @(2h)
He implementado el HUD. Ahora el personaje tiene barra de vida y hay un inventario que indica que arma tiene seleccionada.

## 20250409 MáximoMartínez @(3h)
(2h) En clase de prácticas creé el test para los enemigos que creé ayer y funcionaron perfectamente. Hecho esto fui a descartar la clase antigua ```Enemy``` para usar la nueva actualizando todas las referencias. En el proceso reventó un problema que llevaba siendo arrastrado bastante tiempo que eran 3 referencias circulares: entre ```Enemy``` y ```Character```; otra entre ```Enemy``` y ```Character``` pero a través de ```Weapon```; y finalmente una entre ```Character``` y ```Weapon```. Empecé a trabajar en cómo solucionar este problemón.

(1h) He conseguido aislar el problema de las referencias circulares. Las he eliminado de raíz donde era evidente que no hacían falta y la funcionalidad que se buscaba se debía gestionar a través de InGame. Las funcionalidades que no bastaba con quitar una parte y necesitan reimplementarse las he comentado.

(1h) He cambiado ```Enemy``` para que use SpriteFaçade. A parte he descargado unos sprites para un enemigo murciélago y he implementado la animación de este. Nota: necesitamos un sistema global para animaciones. Olvidé hacer commit de esto último el mismo día.

## 20250410 MáximoMartínez @(4h)
(1h 30min) He implementado una clase ```AnimatedSprite``` para poder usar animaciones de spritesheets con la fachada de sprite. Permite cargar varias animaciones que se identifican por un nombre y elegir los FPS de la reproducción. La he añadido a la clase ```Enemy``` para probarlo.
(1h) He creado la clase ```VisualEffect``` para los futuros efectos visuales que querramos añadir. Hace uso de la clase ```AnimatedSprite``` que hice hace nada. Para manejar los en ```InGame``` más fácilmente he creado la clase ```VFXManager```. He hecho un test y funciona perfectamente.
(1h) He empezado a implementar aquello que descartamos por las referencias circulares (sistema de combate). El jugador tiene ```Hurtbox```, ```InGame``` comprueba si alguna hitbox colisiona con él y recibe daño si eso ocurre.

## 20250410 JorgePelegrin @(3h)
Arregladas todas las referencias circulares, mejorando el sistema de combate y añadiendo cooldown a todos los ataques y habilidades. He empezado a añadir varios items.

## 20250411 EstrellaDominguez @(1h)
Implementado los recursos en la nube mediante el archivo setup.sh y Dropbox.

## 20250411 MáximoMartínez @(4h 20min)
(2h 20min) He puesto un efecto al ataque de los enemigos. He arreglado cómo recibe daño el jugador con un estado de invencibilidad. He conseguido que los enemigos reciban daño. En ese proceso he arreglaod una "Illegal expression" que surgía al obtener la hitbox del arma porque no estaba planteada como un puntero. Al cambiarlo se ha solucionado. También he invertido tiempo en buscar y preparar más assets que añadir al juego.  
(2h) He cambiado el pathfinding. Estaba hecho con las distancias Manhattan y ahora es euclídea. Ahora es más costoso y de vez en cuando tarda más de 15000 microsegundos en calcular (como 1.5frames a 60fps). Por lo que necesita ser optimizarlo.

## 20250412 MáximoMartínez @(3h)

(3h) He modificado el asset del murcielago para que tenga animación de muerte. Antes de añadir un nuevo tipo de enemigo he implementado un ```EnemyManager``` para gestionar los enemigos de ahora en adelante. La clase ```Enemy``` ahora tiene métodos virtuales para que el manager gestione bien las clases hijas. He tardado más de lo esperado por muchos cosas que no comprendía de C++, pero he aprendido una barbaridad. 
(1h) He implementado knockback para todos los enemigos.
(2h 30min) He añadido un enemigo ```Necromancer``` nuevo con diferente comportamiento. También me he ocupado de agregar compatibilidad con diferentes enum de estados entre los hijos y la clase padre ```Enemy```.

## 20250412 EstrellaDominguez @(2h)
He implementado el menu de pausa. Se abre cuando el usuario pulsa la tecla "esc" aunque hay algún error a la hora de mostrarlo.


## 20250412 DavidGonzález @(5h)
Refactorización de la interacción, creando una carpeta a parte y modulando las interacciones por clases, funcionan las interacciones básicas pero no he conseguido que los botones abran la puerta y encima he descubierto que mi codigo para quitar bounds no funciona correctamente. He hecho tambien un mapa de obstaculos para probar pathfindings.

## 20250413 DavidGonzález @(6h)
Refactorización aparentemente funcionando como debe, no vuelvo a refactorizar un codigo en mi vida, los botones abren puerta y ya hay spawnInteraction para mover al jugador entre salas, falta arreglar que setLocaltile quite la colision de los bloques, que me estoy peleando y no la consigo quitar por ahora.

## 20250412 - 20250413 JorgePelegrin @(6h 30min)
He añadido el sistema de karma con las dos ramas, una de pecado y otra de absolucion, y lo he juntado con la tienda para poder comprar las mejoras desde ahí. También he implementado todas las mejoras al personaje y armas, las cuales he tenido que refactorizar un poco el código para poder aplicar las mejoras permanentes a todas las armas a la vez.

## 20250414 JorgePelegrin @(1h)
He movido un poco el arco y la lanza para que no estén tan alejados del personaje, y he añadido la animación de una burbuja a modo visual para poder saber cuando tienes el escudo de la mejora permanente activo y no te pueden golpear. 

## 20250414 MáximoMartínez @(5h 30min)
Me he puesto a solucionar diferentes errores y a trabajar sobre la fachada de sonido que hice en el hito 1 para añadir música y efectos de sonido. Luego he ayudado a un compañero a solucionar ciertos errores. Después, parte del tiempo ha sido una sesión de planificación en grupo para el bucle de salas. 

## 20250414 EstrellaDominguez @(4h)
He cambiado algunos archivos de State para implementarlos con el motor 2D.
He arreglado un error con el menú de pausa.
He implementado los mensajes de texto de los botones y las puertas para ayudar al jugador.
Ahora en el HUD se ve también el arma secundaria del personaje.
Por último he cambiado la barra de vida por corazones.

## 20250414 DavidGonzález @(3h)
Arreglado problema con los bounds, ya se puden eliminar bounds para dar paso al jugador a zonas previamente inaccesibles.

## 20250414 EstrellaDominguez @(30min)
Arreglos menores en la tienda y el HUD. Subida a main para el Hito 2.

## 20250506 DavidGonzález @(2h)
Spawn at funcionando correctamente con ida y vuelta + puliendo world_1

## 20250507 EstrellaDomínguez @(1h)
He implementado la pantalla de KO a la que se redirige cuando el personaje muere. Aunque estéticamente no está correcta y algunas funciones fallan.

## 20250506 JorgePelegrín @(1h)
He hecho un poco de mapa y cambiado la mecánica del tp de la lanza, para que ahora se coloque el portal en la posición del jugador y no en la del ratón.

## 20250508 DavidGonzález @(2h)
World_1 casi terminado (faltan los tps)

## 20250509 JorgePelegrín @(4h 30min)
He añadido varios items para mejorar las armas y alguna que otra mecánica nueva para mejorarlas. También he añadido el sprite del personaje con su animación.

## 20250410 EstrellaDomínguez @(4h)
Ya funciona la ventana de KO y el personaje muere.
Implementación del patrón State para el lobby y mundo 1.
El personaje spawnea en el lobby.
Ahora los menús también funcionan con el clic del ratón y no solo con la tecla Enter.

## 20250409 MaximoMartinez @(3h)
Estuve ayudando a David para gestionar los estados de las salas para definir su bucle y poder empezar con otras tareas como spawnear enemigos. Por mi parte, no se terminó el trabajo en este día porque hubo erroes muy raros muy dificiles de debuggear en temas de casting de subclases.

## 20250511 DavidGonzález @(6h)
Muchas cosas, sistema de estados por interacción para manejar el cambio entre mapas al pasar por zonas, cambios ligeros en los mapas, añadidas funcion clear en musicManager y en TileMap entre otras y debug.

## 20250511 MaximoMartinez @(7h)
Me he ocupado de resolver una referencia circular entre InGame y WorldChangeInteraction. Luego he implementado un sistema con el patrón State para definir las salas de un mundo y así conocer en qué sala se encuentra el jugador y poder spawnear enemigos si es la primera vez que entra o cualquier otra lógica.
(18:40) He trabajado con dicho sistema para que funcione en el mundo 1. Ahora las salas están definidas, si entras a 1 aparecen enemigos (en posiciones estaticas de momento) y cuando son eliminados se puede ejecutar lógica adicional (abrir puertas por ejemplo). He añadido que se puedan pasar funciones por parametro a las salas para así meter las funciones que eliminan sus respectivas puertas sin hacer una clase para cada sala. 
(21:30) He pulido algunas asperezas del sistema anterior. Luego he cambiado las colisiones del personaje principal. En el momento no podía seguir entonces cambié para implementar la transición de canciones. A la tercera va la vencida y ahora las canciones transicionan entre sí. He invertido además un buen rato en crear canciones con IA para rellenar un poco el juego.

## 20250512 DavidGonzález @(7h)
World_2 hecho, completamente funcional con spawn entre salas y puzzle del botón funcionando también porque soy una bestia, añadido World2State también completamente funcional, ahora cuando pasas por la puerta del mundo 2 (en el lobby) te lleva al mundo 2 con su estado y todo (he quitado la barrera de momento para probarlo). Tambien he puesto un tile para retornar al lobby (el 1492 por si quereis hacer pruebas).

## 20250512 MaximoMartinez @(2h)
Arreglé un error por el que las puertas no se abrían al completar un combate, no pasaba el puntero de TileMap.
He solucionado las asperezas del movimiento del personaje, ahora chocar con una pared no anula el movimiento en los dos ejes en movimientos diagonales.
Luego me puse a poner los enemigos en sus sitios para cada sala pero me salió un error muy raro que desapareció solo (me lié o algo).

## 20250513 MaximoMartinez @(1h 30min)
He terminado de colocar los enemigos del primer y segundo mundo. 
He agregado funciones para liberar la memoria de las salas que usaremos más adelante cuando el jugador pueda completar un mundo o morir. 
He invertido tiempo en planificar algunos posibles enemigos que implementar para los siguientes mundos.
He arreglado un problema con la posición del necromancer.

## 20250513 EstrellaDomínguez @(1h)
He comenzado a desarrollar el ten sheet, puesto mi parte de autoevaluación, desarrollado el manual de usuario y dado estructura al archivo de tareas.


## 20250509 JorgePelegrín @(4h 45min)
Añadidos los items al matar todos los enemigos, con sus sprites. Falta añadir un pequeño texto abajo que ponga lo que hace.

## 20250514 MaximoMartinez @(2h)
Haciendo pruebas con los enemigos detecté errores y me puse a solucionarlos. Ya no atraviesan paredes y voy a rehacer el pathfinding. 
He vuelto a invertir tiempo en poner eliminar la decoración de las puertas cuando una sala se completa (antes no estaban).

## 20250509 JorgePelegrín @(1h 30min)
He hecho el HUD un singleton para mejor manejo, y he animado los items poniendo sus nombres encima, además de añadir un HUD temporal explicativo de lo que hace el item que se coge. 

## 20250515 MaximoMartinez @(3h)
Haciendo pruebas con los enemigos detecté errores y me puse a solucionarlos. Ya no atraviesan paredes.
He vuelto a invertir tiempo en eliminar la decoración de las puertas cuando una sala se completa (antes no estaban) y en añadir las puertas del mundo 2.
Por la tarde he cambiado la clase de la sala para permitir distintos tipos de música.

## 20250516 MaximoMartinez @(1h 30min)
Me he puesto a diseñar los bosses y configurar los sprites que elegimos para ellos.
Hecho esto empecé a programar el primero de ellos. No terminé.

## 20250517 MaximoMartinez @(4h 30min)
He seguido trabajando en el primer boss. Ha quedado totalmente implementado. 
Para ello también he implementado un Minion quee es una copia del boss pequeña que el boss genera de vez en cuando. Para terminarlo he tenido que editar AnimatedSprite y el movimiento de los enemigos, porque forzaban la escala del sprite a 1 (no se podía asignar una escala personalizada).
He implementado el tecer boss.

## 20250509 JorgePelegrín @(4h 30min)
He implementado el segundo boss al completo. Lo único que falta es añadirle los efectos de sonidos a sus ataques, etc.

## 20250418 EstrellaDomínguez @(2h)
El estado de KO funciona prácticamente al completo. Ya se restauran todos los items, enemigos, armas, karma, botones y música, sin embargo cuando mueres una segunda vez falla algo.
