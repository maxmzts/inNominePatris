# Log del trabajo realizado por dia
**El log es obligatorio**

## 20230108 FidelAznar @(1h)
Debe contener un sub item como el anterior, con la fecha de la entrada y las cuestiones generales que se realizaron, así como el tiempo total dedicado en la sesión. 

## 20250223 JorgePelegrín @(30min)
Añadí el sprite de la espada al main de la plantilla e hice que siempre estuviera delante del sprite del jugador en las cuatro posiciones.

## 20250224 MaximoMartinez @(1.25h)
He seguido un [tutorial de YouTube](https://www.youtube.com/playlist?list=PL6xSOsbVA1eb_QqMTTcql_3PdOiE928up) para empezar a comprender SFML. Creé una clase ```Game``` que encapsula el motor de juego y la aproveché para entender la gestión de librerías con CMake. No compilaba cuando dejé de trabajar.

## 20250225 MaximoMartinez @(10 min)
Revisé el ```CMakeLists.txt``` y añadí una referencia que me faltó a la librería, conseguí compilar el trabajo de ayer. Además agregé ".vscode/settings.json" al ```.gitignore```.

## 20250226 MaximoMartinez @(1 h)
Antes y durante la clase de prácticas seguí el tutorial que mencioné. No hice commit porque estaba sin terminar y estaba estudiando para otra asigatura durante la semana.

## 20250226 JorgePelegrín @(2h, 30min)
Añadí la clase espada, para empezar a probar el ataque básico y la habilidad. Además, cree una clase de personaje básica y una de enemigo para poder probar el daño de los ataques. Tiene algún error de compilación y falta añadir los sprites bien para que la prueba sea visual.

## 20250228 JorgePelegrín @(1h)
Ya compila, el ataque se ejecuta y el la habilidad también, además funciona. Falta dibujar al enemigo y la espada y probar si el básico funciona y hace daño. El mayor problema es que el personaje no se mueve con las flechas, solo con el dash.

## 20250303 DavidGonzález @(1h, 15min)
He creado el backlog del equipo en github para poder organizar nuestras tareas, además he añadido mis tareas al to do.

## 20250303 EstrellaDomínguez @(1h)
He solucionado errores de ```CMakeList.txt``` y creado la clase ```Interface``` que contiene el código del Menú y el HUD. Además he añadido mis tareas al to do.

## 20250303 EstrellaDomínguez @(30min)
He empezado a implementar el menú. Compila sin errores pero no se ve en pantalla lo que esperaba.

## 20250303 MaximoMartinez @(30min)
Tendría que dejar de hacer tutoriales.

## 20250305 JorgePelegrín @(1h, 45 min)
Hecho un sistema de items, que mejoran las estadísticas del personaje, falta probar si funciona. El personaje ya se mueve, dashea hacia todas las direcciones, y ataca, aunque solo hacia la derecha y la izquierda.

## 20250305 MaximoMartinez @(2h)
Durante prácticas he comenzado a programar un sistema de detección de áreas. Se compone de 2 clases abstractas que sirven de interfaz para cualquier implementación que se les quiera dar. 
- DetectedArea: existe para ser detectado.
- DetectingArea: escanea instancias de DetectedArea dentro de su área.
He hecho una prueba de este sistema en el main.