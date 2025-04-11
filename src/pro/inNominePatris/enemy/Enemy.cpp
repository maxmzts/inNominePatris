#include "Enemy.h"
#include <vector>
#include <queue>
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include <VFXManager.h>

Enemy::Enemy(const std::string& name, float maxHealth, float movementSpeed, const sf::Vector2f& startPosition)
    : name(name)
    , maxHealth(maxHealth)
    , currentHealth(maxHealth)
    , movementSpeed(movementSpeed)
    , position(startPosition)
    , isInvincible(false)
    , invincibilityTimer(0.0f)
    , invincibilityDuration(0.5f)  // Medio segundo de invencibilidad por defecto
    , currentState(EnemyState::IDLE)
    , stateTimer(0.0f)
    , attackDamage(10)          // Valor por defecto
    , attackCooldown(1.0f)         // 1 segundo entre ataques
    , attackTimer(0.0f)
    , detectionRadius(300.0f)      // Detecta al jugador a 300 unidades
    , velocity(0.0f, 0.0f)
    , facingRight(false)
{
    // Creamos los hitboxes y hurtboxes
    // Los tamaños y offsets deben ajustarse según el sprite
    hitbox = new Hitbox(sf::Vector2f(40.0f, 40.0f), sf::Vector2f(10.0f, 10.0f));
    hurtbox = new Hurtbox(sf::Vector2f(60.0f, 60.0f), sf::Vector2f(0.0f, 0.0f));
    
    // Configuración inicial del sprite animado
    sprite.setPosition(position.x, position.y);
    animator = new AnimatedSprite(sprite);
    loadAnimations();

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
    
    changeAnimation(currentState);
}

void Enemy::loadAnimations() {
    std::cout << "Cargo animación\n"; 
    // Animación de vuelo (4 frames en una fila)
    // Parámetros: nombre, frameCount, startPosition(x,y), frameSize(width,height), loop
    animator->addAnimation("idle", 4, sf::Vector2i(0, 0), sf::Vector2i(64, 64), true);
    animator->addAnimation("moving", 4, sf::Vector2i(0, 0), sf::Vector2i(64, 64), true);
    animator->addAnimation("attacking", 4, sf::Vector2i(0, 0), sf::Vector2i(64, 64), true);
    
    // Si hay más animaciones específicas para otros estados, se agregarían aquí
    // Por ejemplo, si hay frames diferentes para el ataque o para recibir daño
    
    // // Configurar callback para cuando termine la animación (si es necesario)
    // animator->setAnimationEndCallback([this]() {
    //     // Por ejemplo, si la animación de ataque termina, volver a IDLE
    //     if (currentState == ATTACKING) {
    //         changeAnimation(IDLE);
    //     }
    // });
}

/**
 * Cambia la animación según el estado actual
 */
void Enemy::changeAnimation(EnemyState newState) {
    currentState = newState;
    stateTimer = 0.0f;
    
    switch (newState) {
        case EnemyState::IDLE:
            animator->play("idle", 8.0f); // 8 FPS para idle
            break;
        case EnemyState::MOVING:
            animator->play("moving", 12.0f); // 10 FPS para movimiento
            break;
        case EnemyState::ATTACKING:
            animator->play("attacking", 24.0f, false); // Animación de ataque no en bucle
            break;
        case EnemyState::HURT:
            animator->play("idle", 1.0f);
            break;
        case EnemyState::DYING:
            animator->play("idle", 1.0f);
            break;
    }
}

void Enemy::takeDamage(float damage) {
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
    changeState(EnemyState::HURT);
    
    // Activar invencibilidad
    setInvincible(true);
    
    // Si la vida llega a 0, cambiar al estado de muerte
    if (currentHealth <= 0) {
        changeState(EnemyState::DYING);
    }
}

/**
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
    changeState(EnemyState::ATTACKING);
    
    // Resetear el timer de ataque
    attackTimer = attackCooldown;
}

void Enemy::move(const sf::Vector2f& direction) {
    // Normalizar el vector de dirección si no es cero
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 0) {
        sf::Vector2f normalizedDir = direction / length;
        velocity = normalizedDir * movementSpeed;

        // Ajusta la dirección del sprite
        if (direction.x > 0 && !facingRight) {
            sprite.setScale(1.f, 1.f);
            facingRight = true;
        } else if (direction.x < 0 && facingRight) {
            sprite.setScale(-1.f, 1.f);
            facingRight = false;
        }
    } else {
        velocity = sf::Vector2f(0.0f, 0.0f);
    }
    
    // El movimiento real se aplica en update
}

// Estructura para representar un nodo en el algoritmo A*
struct PathNode {
    int x, y;           // Coordenadas del nodo en el mapa de tiles
    float g;            // Costo desde el inicio hasta este nodo
    float h;            // Heurística (estimación del costo hasta el objetivo)
    float f;            // Costo total (g + h)
    PathNode* parent;   // Nodo padre en el camino óptimo

    PathNode(int _x, int _y) : x(_x), y(_y), g(0), h(0), f(0), parent(nullptr) {}
    
    // Para comparar nodos en la cola de prioridad
    bool operator < (const PathNode& other) const {
        return f > other.f; // Mayor f significa menor prioridad
    }
    
    // Para comparar si dos nodos son iguales (misma posición)
    bool operator == (const PathNode& other) const {
        return x == other.x && y == other.y;
    }
    bool operator != (const PathNode& other) const {
        return !(*this == other);
    }
};

// Función hash para PathNode para usarlo en unordered_map
struct PathNodeHash {
    std::size_t operator()(const PathNode& node) const {
        return std::hash<int>()(node.x) ^ (std::hash<int>()(node.y) << 1);
    }
};

// Función para calcular la distancia heurística entre dos puntos (distancia Manhattan)
float calculateHeuristic(int x1, int y1, int x2, int y2) {
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}

// Esta función reemplaza la versión básica en Enemy.cpp
void Enemy::findPathToPlayer(const Character* player, const TileMap* tileMap) {
    if (!player || !tileMap) {
        return;
    }
    
    // Obtener la posición del jugador y del enemigo
    sf::Vector2f playerPos = player->getPosition();
    
    // Convertir posiciones del mundo a coordenadas de tiles
    // Suponiendo que cada tile es de 32x32 pixeles (ajustar según tu juego)
    const int TILE_SIZE = 16;
    
    int startX = static_cast<int>(position.x / TILE_SIZE);
    int startY = static_cast<int>(position.y / TILE_SIZE);
    int targetX = static_cast<int>(playerPos.x / TILE_SIZE);
    int targetY = static_cast<int>(playerPos.y / TILE_SIZE);
    
    // Crear nodos de inicio y objetivo
    PathNode startNode(startX, startY);
    PathNode targetNode(targetX, targetY);
    
    // Si el enemigo y el jugador están en el mismo tile, moverse directamente
    if (startX == targetX && startY == targetY) {
        sf::Vector2f direction = playerPos - position;
        move(direction);
        return;
    }
    
    // Conjunto de nodos abiertos (por explorar)
    std::priority_queue<PathNode> openSet;
    
    // Conjunto de nodos ya explorados
    std::unordered_map<PathNode, bool, PathNodeHash> closedSet;
    
    // Mapa de nodos para reconstruir el camino
    std::unordered_map<PathNode, PathNode*, PathNodeHash> cameFrom;
    
    // Inicializar el nodo de inicio
    startNode.g = 0;
    startNode.h = calculateHeuristic(startX, startY, targetX, targetY);
    startNode.f = startNode.g + startNode.h;
    
    // Añadir el nodo de inicio a la lista abierta
    openSet.push(startNode);
    
    // Direcciones posibles de movimiento (4 direcciones: arriba, derecha, abajo, izquierda)
    const int dx[4] = {0, 1, 0, -1};
    const int dy[4] = {-1, 0, 1, 0};
    
    // Vector para almacenar el camino final
    std::vector<sf::Vector2f> path;
    
    // Límite máximo de iteraciones para evitar bucles infinitos
    const int MAX_ITERATIONS = 1000;
    int iterations = 0;
    
    while (!openSet.empty() && iterations < MAX_ITERATIONS) {
        iterations++;
        
        // Obtener el nodo con menor f de la lista abierta
        PathNode current = openSet.top();
        openSet.pop();
        
        // Si hemos llegado al objetivo, reconstruir el camino
        if (current == targetNode) {
            // Reconstruir el camino
            PathNode* currentPtr = cameFrom[current];
            while (currentPtr != nullptr) {
                if (*currentPtr == startNode) break;
                
                // Convertir de coordenadas de tiles a coordenadas del mundo
                path.push_back(sf::Vector2f(currentPtr->x * TILE_SIZE + TILE_SIZE/2, 
                                           currentPtr->y * TILE_SIZE + TILE_SIZE/2));
                
                auto it = cameFrom.find(*currentPtr);
                if (it == cameFrom.end()) break;  // Salir si no hay más camino
                currentPtr = it->second;
            }
            
            // Invertir el camino para que vaya desde el inicio hasta el objetivo
            std::reverse(path.begin(), path.end());
            break;
        }
        
        // Marcar el nodo actual como visitado
        closedSet[current] = true;
        
        // Explorar vecinos
        for (int i = 0; i < 4; i++) {
            int newX = current.x + dx[i];
            int newY = current.y + dy[i];
            
            // Crear el nuevo nodo vecino
            PathNode neighbor(newX, newY);
            
            // Saltar este vecino si ya ha sido explorado
            if (closedSet.find(neighbor) != closedSet.end()) {
                continue;
            }
            
            // Verificar si el tile es transitable
            sf::FloatRect tileRect(newX * TILE_SIZE, newY * TILE_SIZE, TILE_SIZE, TILE_SIZE);
            if (tileMap->isColliding(tileRect)) {
                // Este tile tiene colisión, no es transitable
                closedSet[neighbor] = true;
                continue;
            }
            
            // Calcular el costo g para este vecino
            float tentativeG = current.g + 1.0f; // Costo de moverse a un tile adyacente
            
            // Comprobar si este vecino ya está en la lista abierta
            bool isInOpenSet = false;
            std::priority_queue<PathNode> tempQueue = openSet;
            while (!tempQueue.empty()) {
                if (tempQueue.top() == neighbor) {
                    isInOpenSet = true;
                    break;
                }
                tempQueue.pop();
            }
            
            if (!isInOpenSet || tentativeG < neighbor.g) {
                // Este camino al vecino es mejor, actualizar información
                neighbor.g = tentativeG;
                neighbor.h = calculateHeuristic(newX, newY, targetX, targetY);
                neighbor.f = neighbor.g + neighbor.h;
                
                // Guardar el padre para reconstruir el camino después
                PathNode* neighborPtr = new PathNode(newX, newY);
                *neighborPtr = neighbor;
                PathNode* currentPtr = new PathNode(current.x, current.y);
                *currentPtr = current;
                
                cameFrom[neighbor] = currentPtr;
                
                // Añadir a la lista abierta si no estaba ya
                if (!isInOpenSet) {
                    openSet.push(neighbor);
                }
            }
        }
    }
    
    // Si tenemos un camino, moverse hacia el siguiente punto
    if (!path.empty()) {
        // Tomar el primer punto del camino
        sf::Vector2f nextPoint = path[0];
        
        // Calcular la dirección hacia ese punto
        sf::Vector2f direction = nextPoint - position;
        
        // Moverse en esa dirección
        move(direction);
    } else {
        // Si no hay camino disponible, intentar un movimiento directo
        // (podría ser bloqueado por obstáculos, pero es mejor que no moverse)
        sf::Vector2f direction = playerPos - position;
        move(direction);
    }
    
    // Limpiar la memoria asignada dinámicamente
    for (auto& pair : cameFrom) {
        delete pair.second;
    }
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

// Primero, añade estos miembros a la clase Enemy en el archivo de cabecera (Enemy.h):

void Enemy::update(float deltaTime, Character* player, const TileMap* tileMap) {
    // Actualizar los timers
    if (invincibilityTimer > 0) {
        invincibilityTimer -= deltaTime;
        if (invincibilityTimer <= 0) {
            setInvincible(false);
        }
    }
    
    if (attackTimer > 0) {
        attackTimer -= deltaTime;
    }
    
    // Actualizar el timer del estado actual
    stateTimer += deltaTime;
    
    // Actualizar el timer para recalcular el camino
    pathUpdateTimer += deltaTime;

    animator->update(deltaTime);
    
    // Lógica basada en el estado actual
    switch (currentState) {
        case EnemyState::IDLE:
            // En estado idle, buscar al jugador
            if (player) {
                // Recalcular el camino solo en intervalos específicos
                if (pathUpdateTimer >= pathUpdateInterval) {
                    pathUpdateTimer = 0;
                    findPathToPlayer(player, tileMap);
                }
                
                // Si comenzamos a movernos, cambiar al estado de movimiento
                if (velocity.x != 0 || velocity.y != 0) {
                    changeState(EnemyState::MOVING);
                }
            }
            break;
            
        case EnemyState::MOVING:
            // Aplicar velocidad al movimiento
            position += velocity * deltaTime;
            sprite.setPosition(position.x, position.y);
            updateHitboxes();
            
            // Recalcular el camino periódicamente mientras nos movemos
            if (player && pathUpdateTimer >= pathUpdateInterval) {
                pathUpdateTimer = 0;
                findPathToPlayer(player, tileMap);
            }
            
            // Verificar si podemos atacar al jugador
            if (player) {
                sf::Vector2f playerPos = player->getPosition();
                sf::Vector2f direction = playerPos - position;
                float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                
                // Si estamos lo suficientemente cerca y el ataque no está en cooldown
                if (distance < 50.0f && attackTimer <= 0) {
                    attack();
                }
            }
            break;
            
        case EnemyState::ATTACKING:
            hitbox->setActive(true);
            VFXManager::getInstance().addEffect(
                "./resources/vfx/scratch.png",
                {hitbox->getPosition().x, hitbox->getPosition().y},          // posición de prueba
                {45 , 33},           // tamaño de frame
                8,                  // cantidad de frames
                24.f,                 // FPS
                facingRight
            );
            // La animación de ataque duraría un tiempo fijo
            if (stateTimer >= 0.5f) {  // Duración de la animación de ataque
                changeState(EnemyState::IDLE);
                hitbox->setActive(false);
            }
            break;
            
        case EnemyState::HURT:
            setInvincible(true);
            // La animación de daño duraría un tiempo fijo
            if (stateTimer >= 0.3f) {  // Duración de la animación de daño
                changeState(EnemyState::IDLE);
                setInvincible(false);
            }
            break;
            
        case EnemyState::DYING:
            // La animación de muerte duraría un tiempo fijo
            // Después de ese tiempo, el enemigo debería ser eliminado del juego
            // Esto se manejaría desde el gestor de enemigos
            break;
    }
}

void Enemy::setInvincible(bool invincible) {
    isInvincible = invincible;
    if (invincible) {
        invincibilityTimer = invincibilityDuration;
        
        // Efecto visual de invencibilidad (parpadeo)
        sprite.setColor(sf::Color(255, 255, 255, 128));  // Semi-transparente
    } else {
        sprite.setColor(sf::Color(255, 255, 255, 255));  // Opaco
    }
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

void Enemy::changeState(EnemyState newState) {
    // Si estamos cambiando a un nuevo estado, reiniciar el timer
    if (currentState != newState) {
        currentState = newState;
        stateTimer = 0.0f;
        changeAnimation(newState);
        // Acciones específicas al cambiar de estado podrían ir aquí
        // Por ejemplo, cambiar la animación según el estado
    }
}