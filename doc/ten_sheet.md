# Ten Sheet

## Ficha de juego
### Sistema de juego
Action RPG estilo roguelike con progresión permanente. El juego se divide en varios mundos con jefes finales en cada uno. Completar los dichos mundos desbloquea el jefe final. Cada mundo se compone de distintas salas aleatorias en las que el jugador combate a diferentes enemigos. Dentro de una sala se deben eliminar a todos los enemigos para poder avanzar a la siguiente hasta alcanzar el jefe.Action RPG estilo roguelike con progresión permanente. El juego se divide en varios mundos con jefes finales en cada uno. Completar los dichos mundos desbloquea el jefe final. Cada mundo se compone de distintas salas aleatorias en las que el jugador combate a diferentes enemigos. Dentro de una sala se deben eliminar a todos los enemigos para poder avanzar a la siguiente hasta alcanzar el jefe.

### Público al que va dirigido
- Interesados en juegos difíciles basados en la habilidad.
- Jóvenes que juegan a videojuegos 2D.
- Jugadores que disfrutan de historias con misterio y relacionadas con la jugabilidad.

### Resumen de la historia orientada a la jugabilidad
El protagonista (el padre) busca vengar a su familia, eliminando a aquellos responsables de su muerte. Al final se descubre que él mismo estuvo detrás de la tragedia. Todo el juego resulta ser un purgatorio donde el protagonista está condenado perpetuamente a sufrir la pérdida de su familia y la culpa de haberlo causado.

### Puntos únicos del juego
- Relación estrecha entre historia y mecánicas: como el jugador juega y sus posibilidades están relacionadas con lo que ocurre en el juego y la trama.
- Sistema de karma: El pecado permite al jugador acceder a mejoras relacionadas con la violencia. Mientras tanto, la absolución ofrece mejoras diferentes, basadas en el crecimiento personal.
- Diseño procedural de niveles: Niveles autogenerados y orden aleatorio que dan al jugador una sensación de variedad y refrescan el gameplay.
- Jugador inmerso en el mundo: las acciones del jugador afectan al entorno, involucrándose en el universo ficticio.
- Mecánicas únicas: El arma del jugador es en lo que se basa su gameplay (cómo es, cómo pelea, cómo esquiva). Estas armas están basadas en los jefes.
- Desafío intelectual: Trata al jugador como una persona inteligente, ofreciendo pistas sutiles sobre la historia sin guiarlo de manera directa, fomentando la exploración y la deducción.

### Juegos parecidos
- The Binding of Isaac (2011)
- Undertale (2015)
- Hades (2020)
- Cult of the Lamb (2022)


## Ámbito y jugabilidad
### Bucle de juego
Se compone de las siguientes fases:
1. Inicio: El jugador accede al juego desde el menú principal y selecciona una opción para comenzar la partida. Se presenta un fragmento de la historia para introducir al protagonista, su objetivo y el motivo del juego.
2. Lobby: Se encuentra en una zona segura donde puede personalizar su equipo, revisar misiones o interactuar con NPCs. También es el lugar de acceso a los diferentes mundos.
3. Cruzada en un mundo: Al acceder a un mundo desde el lobby, el jugador es transportado a un nivel o escenario dividido en salas donde debe derrotar enemigos o resolver puzles hasta alcanzar al jefe final del mundo. 
4. Completar la cruzada: Si el jugador derrota al jefe del mundo, recibe unas recompensas y una evaluación de su desempeño.
5. Regreso al lobby y repetición: Tras completar la cruzada o haber perdido durante el proceso, el jugador vuelve al lobby, donde puede prepararse para la siguiente. Se vuelve al punto 2.
6. Fase final: Al haber completado los mundos del juego, se desbloqueará el nivel final, compuesto únicamente de un combate contra el jefe final.
7. Final de juego: Eliminar al jefe final supondrá el final del juego y la resolución de la historia.

### Jugabilidad
#### Progresión
La principal acción a realizar en el juego para progresar es el combate. Luchando en las cruzadas se avanza a través de las salas y la progresión pasa a ser permanente derrotando al jefe final de un mundo. De está manera, obtienes el arma del jefe enemigo, con la que puedes desbloquear el siguiente mundo. 

#### Puzzles
Para aportar variedad, misterio y exploración al gameplay, en el último nivel de cada mundo hay un puzzle que el jugador debe resolver para poder avanzar (por ejemplo, debe encontrar 4 botones repartidos en 4 salas (posiblemente salas fijas), y una vez pulsados desbloquea la puerta que lleva al jefe. O deberá encontrar por el mapa de cada mundo una llave para poder acceder a la sala del jefe de dicho mundo. O debe hablar con un npc que le otorga un objeto necesario para derrotar al jefe).

#### Objetivos del juego
El objetivo final del juego es derrotar al jefe final, al que se podrá acceder tras haber conseguido derrotar a todos los jefes anteriores.

### Cuantificación
El juego se desarrolla principalmente en un purgatorio, un ambiente siniestro y oscuro dividido en diferentes mundos. 
Consta de 3 mundos diferentes con diferentes salas y un jefe cada uno que te premia con un arma si lo vences. Una vez superas los mundos te enfrentas al jefe final.
Dado que cada jefe te proporciona un arma, el personaje dispondrá de 4 armas, 1 por cada jefe más el arma principal que tiene desde el principio.
El juego consta, como se ha comentado anteriormente, de 3 jefes finales, uno por cada mundo, y el propio jefe final. Aparte de estos NPCs, el juego contendrá varios enemigos por cada mundo.


## Interfaz
### Sistema visual
#### HUD y Controles
- Menú de habilidades: Existe un menú informativo abajo a la izquierda en forma de estrella satánica, donde aparecen las habilidades del arma. Otros aspectos como el cooldown de las habilidades del jugador también se visualizan en sus correspondientes espacios de la estrella. Otros datos estadísticos no se muestran hasta el fin de una partida.
- Barras de vida: el jugador no tiene una barra de vida, sino que tiene crucifijos sobre él que representan sus vidas. Para los jefes se mostrará una barra de vida convencional en la parte superior de la pantalla. 

#### Menús
- Menú principal: Menú que contiene la opción de continuar partida, empezar partida nueva, tienda de mejoras, configuración…
- Menú configuración: Menú que contiene opciones de configuración del juego, gráficos, accesibilidad, sonido y controles.
- Menú de mejoras permanentes: Menú que contiene las mejoras del personaje
- Menú in game: Menú que contiene la opción de retomar partida, guardar partida, salir de la partida, ayuda, configuración...

#### Características del motor 2D
Lenguaje: c++ con la librería SFML.
Sistema de compilación: cmake
Posibles implementaciones que se realizarán durante la práctica:
- Sprite Sheets y animaciones: El sistema de animación se basará en spritesheets, permitiendo transiciones fluidas entre estados del personaje y elementos del entorno.
- Colisiones basadas en bounding boxes: Se implementará un sistema de colisiones simple basado en cajas delimitadoras para mejorar la eficiencia del motor.
- Sistema de partículas: Se añadirá un sistema básico de partículas para efectos visuales como explosiones, polvo y destellos.
- Gestión de capas: Se organizan los elementos en diferentes capas para separar fondo, objetos interactuables y personajes.
Sistema de iluminación: soporte para efectos de iluminación y sombreado básico, que mejoren la ambientación del juego.
- Compatibilidad con texturas en formatos estándar: Uso de formatos de imagen como PNG para facilitar la integración de assets gráficos.

#### Cámara
La cámara sigue al personaje mientras se mueve por el mapa. Sin embargo, si el jugador se acerca a los bordes del escenario, la cámara se detiene para no mostrar áreas fuera de los límites del mapa. 
Esto crea un efecto de seguimiento suave que mantiene al personaje centrado en la pantalla, excepto cuando se encuentra cerca de los bordes, donde la cámara se ajusta para mostrar únicamente las zonas permitidas del escenario.

### Controles
- WASD: Movimiento del personaje en 8 direcciones
- Movimiento del ratón: Hay un cursor en la pantalla con el que el jugador apunta para direccionar la acción que realice.
- Acción del arma:
    - Click izquierdo: ataque básico
    - Click derecho: habilidad especial del arma
- B: Comprar mejoras con los puntos del bien y el mal
- E: Interactuar con el entorno

### Sonido
Música hecha con IA o assets sin licencia. Seleccionados con el objetivo de ambientar el juego de manera religiosa y de fantasía oscura, dando una sensación lúgubre, sectaria y misteriosa.
Se incluyen, además, efectos de sonido de las acciones tanto del jugador como de los enemigos.


## Mecánica