# Log del trabajo realizado por día

## 2023-01-08 FidelAznar @(1h)
- **Descripción**: 
  - Se trabajó en tareas generales y se documentó lo realizado.
  
## 2025-02-23 JorgePelegrín @(30min)
- **Descripción**:
  - Añadí el sprite de la espada al main de la plantilla.
  - Hice que la espada siempre estuviera delante del sprite del jugador en las cuatro posiciones.

## 2025-02-24 MaximoMartinez @(1.25h)
- **Descripción**:
  - Seguí un [tutorial de YouTube](https://www.youtube.com/playlist?list=PL6xSOsbVA1eb_QqMTTcql_3PdOiE928up) para empezar a comprender SFML.
  - Creé una clase `Game` que encapsula el motor de juego y la aproveché para entender la gestión de librerías con CMake.
  - El código no compilaba cuando dejé de trabajar.

## 2025-02-25 MaximoMartinez @(10min)
- **Descripción**:
  - Revisé el `CMakeLists.txt` y añadí una referencia faltante a la librería.
  - Conseguí compilar el trabajo de ayer.
  - Añadí `.vscode/settings.json` al `.gitignore`.

## 2025-02-26 MaximoMartinez @(1h)
- **Descripción**:
  - Antes y durante la clase de prácticas, seguí el tutorial mencionado.
  - No hice commit ya que el trabajo estaba sin terminar y estuve estudiando para otra asignatura durante la semana.

## 2025-02-26 JorgePelegrín @(2h, 30min)
- **Descripción**:
  - Añadí la clase de espada para empezar a probar el ataque básico y la habilidad.
  - Creé una clase de personaje básica y una de enemigo para poder probar el daño de los ataques.
  - Se presentaron algunos errores de compilación y falta de integración de los sprites para la visualización.

## 2025-02-28 JorgePelegrín @(1h)
- **Descripción**:
  - El proyecto ya compila, el ataque básico y la habilidad funcionan.
  - Falta dibujar al enemigo y la espada, y probar si el ataque básico hace daño.
  - El principal problema es que el personaje no se mueve con las flechas, solo con el dash.

## 2025-03-03 DavidGonzález @(1h, 15min)
- **Descripción**:
  - Creé el backlog del equipo en GitHub para organizar las tareas.
  - Añadí mis tareas al "To Do".

## 2025-03-03 EstrellaDomínguez @(1h)
- **Descripción**:
  - Solucioné errores de `CMakeLists.txt`.
  - Creé la clase `Interface` que contiene el código del Menú y el HUD.
  - Añadí mis tareas al "To Do".

## 2025-03-03 EstrellaDomínguez @(30min)
- **Descripción**:
  - Empecé a implementar el menú.
  - Compila sin errores, pero no se ve en pantalla lo que esperaba.

## 2025-03-03 MaximoMartinez @(30min)
- **Descripción**:
  - Debería dejar de hacer tutoriales.

## 20250305 MaximoMartinez @(2h)
- **Descripción**: 
  - Durante prácticas he comenzado a programar un sistema de detección de áreas. Se compone de 2 clases abstractas que sirven de interfaz para cualquier implementación que se les quiera dar. 
    - DetectedArea: existe para ser detectado.
    - DetectingArea: escanea instancias de DetectedArea dentro de su área.
  - He hecho una prueba de este sistema en el main.

## 2025-03-05 JorgePelegrín @(2h, 45min)
- **Descripción**:
  - Implementé un sistema de items que mejoran las estadísticas del personaje.
  - Falta probar si el sistema funciona correctamente.
  - El personaje ya se mueve, realiza dash en todas las direcciones y ataca, aunque solo hacia la derecha y la izquierda.
  - *(1 hora despúes de los anteriores)*
  - El personaje ya ataca hacia todas las direcciones
  - Implementado el pickUp de item, probado con uno para aumentar la distancia del dash
  - Cambio un poco de la estructura del log para que se vea mejor estructurado. 

