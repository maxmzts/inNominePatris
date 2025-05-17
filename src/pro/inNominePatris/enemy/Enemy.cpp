#include "Enemy.h"
#include <vector>
#include <queue>
#include <unordered_map>
#include <cmath>
#include "../hboxes/Hitbox.h"
#include "../hboxes/Hurtbox.h"
#include <algorithm>
#include <VFXManager.h>
#include <unordered_set>
#include <SFXManager.h>

Enemy::Enemy(const std::string& name, float maxHealth, float movementSpeed, const sf::Vector2f& startPosition, const std::string& texturePath)
    : name(name)
    , maxHealth(maxHealth)
    , currentHealth(maxHealth)
    , movementSpeed(movementSpeed)
    , position(startPosition * 16.f)
    , isInvincible(false)
    , invincibilityTimer(0.0f)
    , invincibilityDuration(0.5f)  // Medio segundo de invencibilidad por defecto
    , currentState(EnemyState::IDLE)
    , stateTimer(0.0f)
    , attackDamage(1.f)          // Valor por defecto
    , attackCooldown(1.0f)         // 1 segundo entre ataques
    , attackTimer(0.0f)
    , detectionRadius(300.0f)      // Detecta al jugador a 300 unidades
    , velocity(0.0f, 0.0f)
    , facingRight(false)
    , knockbackDirection(0.0f, 0.0f)
    , knockbackForce(0.0f)
    , knockbackDuration(0.3f)
    , knockbackTimer(0.0f)
    , isInKnockback(false)
{
    // Configuración inicial del sprite animado
    setTexture(texturePath);
    sprite.setPosition(position.x, position.y);
    animator = new AnimatedSprite(sprite);

    // Creamos los hitboxes y hurtboxes
    // Los tamaños y offsets deben ajustarse según el sprite
    hitbox = new Hitbox(sf::Vector2f(40.0f, 40.0f), sf::Vector2f(10.0f, 10.0f));
    hurtbox = new Hurtbox(sf::Vector2f(32.f, 32.f), sf::Vector2f(0.f, 0.f));

    updateHitboxes();
}

Enemy::~Enemy() {
    delete hitbox;
    delete hurtbox;
    delete animator;
}

void Enemy::setPosition(const sf::Vector2f& newPosition) {
    position = newPosition;
    sprite.setPosition(position.x, position.y);
    updateHitboxes();
}

/**
 * Carga una textura para aplicar sobre el Façade de sf::Sprite.
 */
void Enemy::setTexture(const std::string& texturePath) {
    sprite.loadTexture(texturePath); 
    sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
    
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
}

void Enemy::setupKnockback(const sf::Vector2f& attackDirection, float force) {
    // Si hay una dirección de ataque, calcular la dirección opuesta
    if (attackDirection.x != 0 || attackDirection.y != 0) {
        // Normalizar la dirección
        float length = std::sqrt(attackDirection.x * attackDirection.x + attackDirection.y * attackDirection.y);
        if (length > 0) {
            // Dirección opuesta al ataque
            knockbackDirection = -attackDirection / length;
        } else {
            // Si no hay dirección clara, usar un valor por defecto
            knockbackDirection = sf::Vector2f(facingRight ? -1.0f : 1.0f, 0.0f);
        }
    } else {
        // Si no se proporciona dirección, usar la dirección contraria a donde está mirando
        knockbackDirection = sf::Vector2f(facingRight ? -1.0f : 1.0f, 0.0f);
    }
    
    knockbackForce = force;
    knockbackDuration = 0.3f; // Duración del knockback en segundos
    knockbackTimer = knockbackDuration;
    isInKnockback = true;
}
/**
 * Método default de ataque (puede ser sobrescrito porque es virtual)
 * Recibe daño, pasa a estado herido y aplica knockback.
 */
void Enemy::takeDamage(float damage, const sf::Vector2f& attackPosition) {
    // Si está invencible, ignorar el daño
    if (isInvincible) {
        return;
    }
    
    currentHealth -= damage;
    
    // Asegurar que la vida no baje de 0
    if (currentHealth < 0) {
        currentHealth = 0;
    }
    
    // Cambiar al estado de herido
    changeState(static_cast<int>(EnemyState::HURT));
    
    // Activar invencibilidad
    setInvincible(true);

    sf::Vector2f attackDirection = attackPosition-position;
    setupKnockback(attackDirection, 150.0f);

    float pitch = 0.8f + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (1.2f - 0.8f);
    SFXManager::getInstance().addEffect("resources/sfx/hit.wav", 60.f, pitch);
    
    // Si la vida llega a 0, cambiar al estado de muerte
    if (currentHealth <= 0) {
        changeState(static_cast<int>(EnemyState::DYING));
    }
}

void Enemy::knockback(float deltaTime, const TileMap* tileMap){
    if (isInKnockback) {
        // La fuerza se reduce con el tiempo para un efecto de desaceleración
        float currentForce = knockbackForce * (knockbackTimer / knockbackDuration);
        sf::Vector2f knockbackVelocity = knockbackDirection * currentForce;
        
        // Mover al enemigo
        position += knockbackVelocity * deltaTime;
        sprite.setPosition(position.x, position.y);
        updateHitboxes();
        
        // Comprobar colisiones con el mapa si está disponible
        if (tileMap) {
            sf::FloatRect enemyBounds = sprite.getGlobalBounds();
            if (tileMap->isColliding(enemyBounds)) {
                // Si colisiona con el mapa, detener el knockback
                isInKnockback = false;
            }
        }
    }
}

/**
 * Método default de ataque (puede ser sobrescrito porque es virtual)
 * Para atacar el enemigo pone activa la hitbox durante un corto periodo de tiempo.
 * El daño producido al jugador se manejará en el sistema de colisiones de InGame.
 * Que la hitbox vuelva a estar inactiva se gestiona en el update.
 */
void Enemy::attack() {
    // Verificar si el ataque está en cooldown
    if (attackTimer > 0) {
        return;
    }
    
    // Cambiar al estado de ataque
    changeState(static_cast<int>(EnemyState::ATTACKING));
    
    // Resetear el timer de ataque
    attackTimer = attackCooldown;
}

void Enemy::calculateVelocity(const sf::Vector2f& direction) {
    // Normalizar el vector de dirección si no es cero
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 0) {
        sf::Vector2f normalizedDir = direction / length;
        velocity = normalizedDir * movementSpeed;

        // Ajusta la dirección del sprite
        if (direction.x > 0 && !facingRight) {
            sf::Vector2f scale = sprite.getScale();
            if(scale.x < 0)
                scale.x = scale.x*(-1.f);
            sprite.setScale(scale.x, scale.y);
            facingRight = true;
        } else if (direction.x < 0 && facingRight) {
            sf::Vector2f scale = sprite.getScale();
            sprite.setScale(scale.x*(-1.f), scale.y);
            facingRight = false;
        }
    } else {
        velocity = sf::Vector2f(0.0f, 0.0f);
    }
    
    // El movimiento real se aplica en update
}


/***********************************************************/
//          PATHFINDING                                    //
/***********************************************************/

// Estructura para representar un nodo en el algoritmo A*
struct PathNode {
    int x, y;           // Coordenadas del nodo en el mapa de tiles
    float g;            // Costo desde el inicio hasta este nodo
    float h;            // Heurística (estimación del costo hasta el objetivo)
    float f;            // Costo total (g + h)
    int parentKey;      // Clave del nodo padre en el camino óptimo
    bool hasParent;     // Indica si el nodo tiene un padre

    PathNode() : x(0), y(0), g(0), h(0), f(0), parentKey(0), hasParent(false) {}
    PathNode(int _x, int _y) : x(_x), y(_y), g(0), h(0), f(0), parentKey(0), hasParent(false) {}
    
    // Para comparar nodos en la cola de prioridad
    bool operator < (const PathNode& other) const {
        return f > other.f; // Mayor f significa menor prioridad
    }
};

// Función para calcular la distancia heurística entre dos puntos (distancia Euclídea)
inline float calculateHeuristic(int x1, int y1, int x2, int y2) {
    // Utilizamos una aproximación más rápida de la distancia euclídea
    int dx = x2 - x1;
    int dy = y2 - y1;
    return std::sqrt(dx * dx + dy * dy);
}

// Función para calcular un índice único para cada posición
inline int getNodeKey(int x, int y) {
    return (x << 16) | (y & 0xFFFF);
}

// Esta función reemplaza la versión básica en Enemy.cpp
void Enemy::findPathToPlayer(const Character* player, const TileMap* tileMap) {
    sf::Clock totalClock; // Reloj para medir el tiempo total
    
    if (!player || !tileMap) {
        return;
    }
    
    // Obtener la posición del jugador y del enemigo
    sf::Vector2f playerPos = player->getPosition();
    
    // Convertir posiciones del mundo a coordenadas de tiles
    const int TILE_SIZE = 16;
    
    int startX = static_cast<int>(position.x / TILE_SIZE);
    int startY = static_cast<int>(position.y / TILE_SIZE);
    int targetX = static_cast<int>(playerPos.x / TILE_SIZE);
    int targetY = static_cast<int>(playerPos.y / TILE_SIZE);
    
    sf::Clock initClock;
    // Si el enemigo y el jugador están en el mismo tile, moverse directamente
    if (startX == targetX && startY == targetY) {
        sf::Vector2f direction = playerPos - position;
        calculateVelocity(direction);
        // std::cout << "Inicialización completada: " << initClock.getElapsedTime().asMicroseconds() << " microsegundos" << std::endl;
        return;
    }
    // std::cout << "Inicialización completada: " << initClock.getElapsedTime().asMicroseconds() << " microsegundos" << std::endl;
    
    // Verificar si hay línea de visión directa (optimización temprana)
    sf::Clock lineOfSightClock;
    sf::Vector2f directDirection = playerPos - position;
    float distanceToPlayer = std::sqrt(directDirection.x * directDirection.x + directDirection.y * directDirection.y);
    
    // Solo hacemos raycast si el jugador está relativamente cerca
    if (distanceToPlayer < TILE_SIZE * 60.f) {
        bool lineOfSight = true;
        float stepSize = TILE_SIZE * 2.f;
        sf::Vector2f rayPos = position;
        sf::Vector2f normalizedDir;
        
        if (distanceToPlayer > 0) {
            normalizedDir = directDirection / distanceToPlayer;
        }
        
        for (float step = 0; step < distanceToPlayer; step += stepSize) {
            rayPos += normalizedDir * stepSize;
            sf::FloatRect pointRect(rayPos.x - 1, rayPos.y - 1, 2, 2);
            
            if (tileMap->isColliding(pointRect)) {
                lineOfSight = false;
                break;
            }
        }
        
        // Si hay línea de visión, moverse directamente sin calcular camino
        if (lineOfSight) {
            calculateVelocity(normalizedDir);
            // std::cout << "Verificación de línea de visión completada: " << lineOfSightClock.getElapsedTime().asMicroseconds() << " microsegundos" << std::endl;
            // std::cout << "Tiempo total de pathfinding: " << totalClock.getElapsedTime().asMicroseconds() << " microsegundos (" 
                    //   << totalClock.getElapsedTime().asMicroseconds() / 16667.0f << " frames a 60 FPS)" << std::endl;
            return;
        }
    }
    // std::cout << "Verificación de línea de visión completada: " << lineOfSightClock.getElapsedTime().asMicroseconds() << " microsegundos" << std::endl;
    
    
    // Limitar el área de búsqueda para mejorar el rendimiento
    sf::Clock distanceCheckClock;
    const int MAX_SEARCH_RADIUS = 20; // Limitar la búsqueda a un radio razonable
    
    // Comprobar si el objetivo está demasiado lejos para buscar
    float distanceToTargetTile = calculateHeuristic(startX, startY, targetX, targetY);
    if (distanceToTargetTile > MAX_SEARCH_RADIUS) {
        // Si está demasiado lejos, moverse en la dirección general
        float angle = std::atan2(targetY - startY, targetX - startX);
        sf::Vector2f direction(std::cos(angle), std::sin(angle));
        calculateVelocity(direction);
        // std::cout << "Verificación de distancia máxima completada: " << distanceCheckClock.getElapsedTime().asMicroseconds() << " microsegundos" << std::endl;
        // std::cout << "Tiempo total de pathfinding: " << totalClock.getElapsedTime().asMicroseconds() << " microsegundos (" 
                //   << totalClock.getElapsedTime().asMicroseconds() / 16667.0f << " frames a 60 FPS)" << std::endl;
        return;
    }
    // std::cout << "Verificación de distancia máxima completada: " << distanceCheckClock.getElapsedTime().asMicroseconds() << " microsegundos" << std::endl;
    
    // Inicializar estructuras para A*
    sf::Clock aStarInitClock;
    std::priority_queue<PathNode> openSet;
    std::unordered_set<int> closedSet;
    std::unordered_map<int, PathNode> allNodes;
    
    // Direcciones posibles de movimiento (8 direcciones: incluye diagonales)
    const int dx[8] = {0, 1, 1, 1, 0, -1, -1, -1};
    const int dy[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
    
    // Inicializar el nodo de inicio
    PathNode startNode(startX, startY);
    startNode.g = 0;
    startNode.h = calculateHeuristic(startX, startY, targetX, targetY);
    startNode.f = startNode.g + startNode.h;
    
    // Añadir el nodo de inicio
    int startKey = getNodeKey(startX, startY);
    allNodes[startKey] = startNode;
    openSet.push(startNode);
    
    // Vector para almacenar el camino final
    std::vector<sf::Vector2f> path;
    
    // Límites para evitar cálculos excesivos
    const int MAX_ITERATIONS = 300; // Reducido para mejor rendimiento
    const int targetKey = getNodeKey(targetX, targetY);
    int iterations = 0;
    bool pathFound = false;
    
    // Matriz precalculada para costos de movimiento
    const float moveCosts[8] = {1.0f, 1.414f, 1.0f, 1.414f, 1.0f, 1.414f, 1.0f, 1.414f};
    // std::cout << "Inicialización de A* completada: " << aStarInitClock.getElapsedTime().asMicroseconds() << " microsegundos" << std::endl;
    
    // Bucle principal del algoritmo A*
    sf::Clock aStarMainLoopClock;
    while (!openSet.empty() && iterations < MAX_ITERATIONS) {
        iterations++;
        
        // Obtener el nodo con menor f de la lista abierta
        PathNode current = openSet.top();
        openSet.pop();
        
        int currentKey = getNodeKey(current.x, current.y);
        
        // Evitar procesar nodos ya visitados
        if (closedSet.find(currentKey) != closedSet.end()) {
            continue;
        }
        
        // Si hemos llegado al objetivo
        if (currentKey == targetKey) {
            pathFound = true;
            break;
        }
        
        // Marcar como visitado
        closedSet.insert(currentKey);
        
        // Explorar vecinos
        for (int i = 0; i < 8; i++) {
            int newX = current.x + dx[i];
            int newY = current.y + dy[i];
            
            // Comprobar límites del mapa si es necesario (implementación opcional)
            // if (newX < 0 || newY < 0 || newX >= mapWidth || newY >= mapHeight) continue;
            
            int neighborKey = getNodeKey(newX, newY);
            
            // Saltar nodos ya visitados
            if (closedSet.find(neighborKey) != closedSet.end()) {
                continue;
            }
            
            // Verificar colisiones sólo si es necesario
            sf::FloatRect tileRect(newX * TILE_SIZE, newY * TILE_SIZE, TILE_SIZE, TILE_SIZE);
            if (tileMap->isColliding(tileRect)) {
                closedSet.insert(neighborKey);  // Marcarlo como cerrado para no revisarlo de nuevo
                continue;
            }
            
            // Para movimientos diagonales, verificar que no haya corte de esquinas
            if (i == 1 || i == 3 || i == 5 || i == 7) {
                int horizontal = (i == 1 || i == 3) ? 1 : -1;
                int vertical = (i == 1 || i == 7) ? -1 : 1;
                
                sf::FloatRect rect1((current.x + horizontal) * TILE_SIZE, current.y * TILE_SIZE, TILE_SIZE, TILE_SIZE);
                sf::FloatRect rect2(current.x * TILE_SIZE, (current.y + vertical) * TILE_SIZE, TILE_SIZE, TILE_SIZE);
                
                if (tileMap->isColliding(rect1) || tileMap->isColliding(rect2)) {
                    continue;
                }
            }
            
            // Calcular costo del camino
            float tentativeG = current.g + moveCosts[i];
            
            // Obtener o crear el nodo vecino
            auto nodeIt = allNodes.find(neighborKey);
            bool isNewNode = nodeIt == allNodes.end();
            
            if (isNewNode) {
                PathNode neighbor(newX, newY);
                neighbor.g = tentativeG;
                neighbor.h = calculateHeuristic(newX, newY, targetX, targetY);
                neighbor.f = neighbor.g + neighbor.h;
                neighbor.parentKey = currentKey;
                neighbor.hasParent = true;
                
                allNodes[neighborKey] = neighbor;
                openSet.push(neighbor);
            } else if (tentativeG < nodeIt->second.g) {
                // Actualizar el nodo existente con un mejor camino
                nodeIt->second.g = tentativeG;
                nodeIt->second.f = tentativeG + nodeIt->second.h;
                nodeIt->second.parentKey = currentKey;
                nodeIt->second.hasParent = true;
                
                // Necesitamos volver a añadirlo a la cola para reconsiderar
                openSet.push(nodeIt->second);
            }
        }
    }
    // std::cout << "Bucle principal de A* completado: " << aStarMainLoopClock.getElapsedTime().asMicroseconds() 
            //   << " microsegundos (iteraciones: " << iterations << ")" << std::endl;
    
    // Reconstruir el camino si se encontró uno
    sf::Clock pathReconstructionClock;
    if (pathFound) {
        // Reconstruir desde el objetivo hacia atrás
        std::vector<sf::Vector2f> reversePath;
        int currentKey = targetKey;
        
        // Sólo necesitamos el primer punto del camino para movernos
        int steps = 0;
        const int MAX_PATH_STEPS = 2; // Limitar la reconstrucción para mejorar rendimiento
        
        while (currentKey != startKey && steps < MAX_PATH_STEPS) {
            auto it = allNodes.find(currentKey);
            if (it == allNodes.end() || !it->second.hasParent) break;
            
            PathNode& node = it->second;
            currentKey = node.parentKey;
            
            if (currentKey != startKey) {
                auto parentIt = allNodes.find(currentKey);
                if (parentIt != allNodes.end()) {
                    PathNode& parentNode = parentIt->second;
                    reversePath.push_back(sf::Vector2f(
                        parentNode.x * TILE_SIZE + TILE_SIZE/2,
                        parentNode.y * TILE_SIZE + TILE_SIZE/2
                    ));
                }
            }
            
            steps++;
        }
        
        // Invertir para obtener el camino correcto
        path.assign(reversePath.rbegin(), reversePath.rend());
    }
    // std::cout << "Reconstrucción del camino completada: " << pathReconstructionClock.getElapsedTime().asMicroseconds() << " microsegundos" << std::endl;
    
    // Decidir cómo moverse
    sf::Clock movementClock;
    if (!path.empty()) {
        // Moverse al primer punto del camino
        sf::Vector2f nextPoint = path[0];
        sf::Vector2f direction = nextPoint - position;
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        
        if (length > 0) {
            direction /= length;
        }
        
        calculateVelocity(direction);
    } else {
        // No se encontró camino o el objetivo está demasiado lejos
        // Usar un movimiento aleatorio para evitar quedarse atascado
        static int stuckCounter = 0;
        stuckCounter++;
        
        if (stuckCounter > 5) {
            int randomDir = rand() % 8;
            sf::Vector2f randomDirection(dx[randomDir], dy[randomDir]);
            float length = std::sqrt(randomDirection.x * randomDirection.x + randomDirection.y * randomDirection.y);
            
            if (length > 0) {
                randomDirection /= length;
            }
            
            calculateVelocity(randomDirection);
            stuckCounter = 0;
        } else {
            // Intentar moverse hacia el jugador de todas formas
            sf::Vector2f direction = playerPos - position;
            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            
            if (length > 0) {
                direction /= length;
            }
            
            calculateVelocity(direction);
        }
    }
    // std::cout << "Cálculo de movimiento completado: " << movementClock.getElapsedTime().asMicroseconds() << " microsegundos" << std::endl;
    
    // Tiempo total del algoritmo
    sf::Int64 totalTime = totalClock.getElapsedTime().asMicroseconds();
    float framesAt60FPS = totalTime / 16667.0f; // 1000000 / 60 = 16667 microsegundos por frame a 60 FPS
    
    // std::cout << "Tiempo total de pathfinding: " << totalTime << " microsegundos (" 
            //   << framesAt60FPS << " frames a 60 FPS)" << std::endl 
            //   << "---------------------------------------------------------------------------------------"
            //   << std::endl;
}

/**
 * Funcion de dibujado de los enemigos.
 */
void Enemy::render(sf::RenderWindow& window) {
    // Para debugging, podemos dibujar los hitboxes y hurtboxes
    hitbox->render(window);
    hurtbox->render(window);
    
    // Dibujar el sprite del enemigo
    sprite.draw(window);
}

// Función para mover los enemigos, gestiona colisiones
void Enemy::move(const TileMap* tileMap, float deltaTime){
    sf::FloatRect nextBounds = sprite.getGlobalBounds();
    sf::FloatRect nextXBounds = nextBounds;
    sf::FloatRect nextYBounds = nextBounds;

    // Comprobación de colisiones antes de mover al personaje
    nextBounds = sprite.getGlobalBounds();

    // Movimiento en eje X
    nextXBounds = nextBounds;
    nextXBounds.left += velocity.x * deltaTime;
    if (tileMap->isColliding(nextXBounds)) {
        velocity.x = 0;
    }

    // Movimiento en eje Y
    nextYBounds = nextBounds;
    nextYBounds.top += velocity.y * deltaTime;
    if (tileMap->isColliding(nextYBounds)) {
        velocity.y = 0;
    }
    position += velocity * deltaTime;
    sprite.move(velocity * deltaTime);
}

// Primero, añade estos miembros a la clase Enemy en el archivo de cabecera (Enemy.h):

// void Enemy::update(float deltaTime, Character* player, const TileMap* tileMap) {
//     // Actualizar los timers
//     if (invincibilityTimer > 0) {
//         invincibilityTimer -= deltaTime;
//         if (invincibilityTimer <= 0) {
//             setInvincible(false);
//         }
//     }
    
//     if (attackTimer > 0) {
//         attackTimer -= deltaTime;
//     }
    
//     // Actualizar el timer del estado actual
//     stateTimer += deltaTime;
    
//     // Actualizar el timer para recalcular el camino
//     pathUpdateTimer += deltaTime;

//     animator->update(deltaTime);
    
//     // Lógica basada en el estado actual
//     switch (currentState) {
//         case EnemyState::IDLE:
//             // En estado idle, buscar al jugador
//             if (player) {
//                 // Recalcular el camino solo en intervalos específicos
//                 if (pathUpdateTimer >= pathUpdateInterval) {
//                     pathUpdateTimer = 0;
//                     findPathToPlayer(player, tileMap);
//                 }
                
//                 // Si comenzamos a movernos, cambiar al estado de movimiento
//                 if (velocity.x != 0 || velocity.y != 0) {
//                     changeState(EnemyState::MOVING);
//                 }
//             }
//             break;
            
//         case EnemyState::MOVING:
//             // Aplicar velocidad al movimiento
//             position += velocity * deltaTime;
//             sprite.setPosition(position.x, position.y);
//             updateHitboxes();
            
//             // Recalcular el camino periódicamente mientras nos movemos
//             if (player && pathUpdateTimer >= pathUpdateInterval) {
//                 pathUpdateTimer = 0;
//                 findPathToPlayer(player, tileMap);
//             }
            
//             // Verificar si podemos atacar al jugador
//             if (player) {
//                 sf::Vector2f playerPos = player->getPosition();
//                 sf::Vector2f direction = playerPos - position;
//                 float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                
//                 // Si estamos lo suficientemente cerca y el ataque no está en cooldown
//                 if (distance < 50.0f && attackTimer <= 0) {
//                     attack();
//                 }
//             }
//             break;
            
//         case EnemyState::ATTACKING:
//             hitbox->setActive(true);
//             VFXManager::getInstance().addEffect(
//                 "./resources/vfx/scratch.png",
//                 {hitbox->getPosition().x, hitbox->getPosition().y},          // posición de prueba
//                 {45 , 33},           // tamaño de frame
//                 8,                  // cantidad de frames
//                 24.f,                 // FPS
//                 facingRight
//             );
//             // La animación de ataque duraría un tiempo fijo
//             if (stateTimer >= 0.5f) {  // Duración de la animación de ataque
//                 changeState(EnemyState::IDLE);
//                 hitbox->setActive(false);
//             }
//             break;
            
//         case EnemyState::HURT:
//             if(hitbox->isActive())
//                 hitbox->setActive(false);
//             setInvincible(true);
//             // La animación de daño duraría un tiempo fijo
//             if (stateTimer >= 0.3f) {  // Duración de la animación de daño
//                 changeState(EnemyState::IDLE);
//                 setInvincible(false);
//             }
//             break;
            
//         case EnemyState::DYING:
//             if(hitbox->isActive())
//                 hitbox->setActive(false);
//             hurtbox->setActive(false);
//             // La animación de muerte duraría un tiempo fijo
//             // Después de ese tiempo, el enemigo debería ser eliminado del juego
//             // Esto se manejaría desde el gestor de enemigos
//             break;
//     }
// }

void Enemy::setInvincible(bool invincible) {
    isInvincible = invincible;
    if (invincible) {
        invincibilityTimer = invincibilityDuration;
        
        // Efecto visual de invencibilidad 
        sprite.setColor(sf::Color(255, 76, 76, 128));  // Semi-transparente
    } else {
        sprite.setColor(sf::Color(255, 255, 255, 255));  // Opaco
    }
}

bool Enemy::isDead() const{ 
    if(currentState == EnemyState::DEAD)
        return true;
    else 
        return false; 
}

void Enemy::updateHitboxes() {
    // Actualizar las posiciones de los hitboxes y hurtboxes
    if (hitbox) {
        hitbox->setPosition(position + sf::Vector2f(10.0f, 10.0f));  // Ajustar según el sprite
    }
    
    if (hurtbox) {
        hurtbox->setPosition(position);  // Centrado en la posición del enemigo
    }
}

void Enemy::changeState(int newStateInt) {
    if (!isValidEnemyState(newStateInt)) return;
    EnemyState newState = static_cast<EnemyState>(newStateInt);

    // Si estamos cambiando a un nuevo estado, reiniciar el timer
    if (currentState != newState) {
        currentState = newState;
        stateTimer = 0.0f;
        changeAnimation(newStateInt);
        // Acciones específicas al cambiar de estado podrían ir aquí
        // Por ejemplo, cambiar la animación según el estado
    }
}

bool Enemy::isValidEnemyState(int state) {
    return state >= static_cast<int>(EnemyState::IDLE) &&
           state <= static_cast<int>(EnemyState::DEAD);
}