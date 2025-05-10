#include "Character.h"
#include "../hboxes/Hurtbox.h"
#include "InteractionFactory.h"
#include <iostream>
#include <cmath>

Character::Character() 
    : speed(200.f), acceleration(1500.f), deceleration(2000.f), equippedWeapon(nullptr), 
      isDashing(false), dashSpeed(400.f), dashDuration(0.2f), weapons(), equippedIndex(0), 
      direction(0.f, 0.f), maxHealth(3), currentHealth(3), isInvencible(false),
      currentState(AnimationState::IDLE_DOWN), wasMoving(false) {

    velocity = sf::Vector2f(0.f, 0.f);

    hurtbox = new Hurtbox(sf::Vector2f(60.0f, 60.0f), sf::Vector2f(0.0f, 0.0f));

    if (!sprite.loadTexture("./resources/Character/South-idle.png")) {
        std::cerr << "Error: No se pudo cargar la textura inicial del personaje" << std::endl;
        exit(1);
    }

    sprite.setScale(2.2f,2.2f); // Escalar el sprite del personaje
    sprite.setOrigin(13.f, 10.f); // Ajustar el origen del sprite para centrarlo

    if (!shieldsprite.loadTexture("./resources/bubble.png")) {
        std::cerr << "Error: No se pudo cargar la textura del escudo" << std::endl;
        exit(1);
    }

    // Inicializar la animación del escudo
    shieldAnimation = new AnimatedSprite(shieldsprite); // Usar el sprite como referencia
    shieldAnimation->addAnimation("shield", 7, {0, 0}, {64, 64}, true); // 7 frames, 64x64 cada uno
    
    // Inicializar sistema de animación del personaje
    animations = new AnimatedSprite(sprite);
    
    // Configurar las rutas de los archivos de animación
    animationTextures = {
        {AnimationState::IDLE_DOWN, "./resources/Character/South-idle.png"},
        {AnimationState::IDLE_UP, "./resources/Character/North-idle.png"},
        {AnimationState::IDLE_LEFT, "./resources/Character/Left-idle.png"},
        {AnimationState::IDLE_RIGHT, "./resources/Character/Right-idle.png"},
        {AnimationState::WALK_DOWN, "./resources/Character/South-Run.png"},
        {AnimationState::WALK_UP, "./resources/Character/North-Run.png"},
        {AnimationState::WALK_LEFT, "./resources/Character/Left-Run.png"},
        {AnimationState::WALK_RIGHT, "./resources/Character/Right-Run.png"}
    };
    
    // Configurar animaciones
    setupAnimations();
    
    // Iniciar con animación idle
    animations->play("idle_down", 8.0f, true);
}

Character::~Character() {
    delete animations;
    delete shieldAnimation;
    delete hurtbox;
}

void Character::setupAnimations() {
    // Añadir todas las animaciones (12 frames de 64x64 por animación)
    animations->addAnimation("idle_down", 10, {0, 0}, {25, 26}, true);
    animations->addAnimation("idle_up", 10, {0, 0}, {25, 26}, true);
    animations->addAnimation("idle_left", 10, {0, 0}, {20, 27}, true);
    animations->addAnimation("idle_right", 10, {0, 0}, {20, 27}, true);
    animations->addAnimation("walk_down", 12, {0, 0}, {25, 25}, true);
    animations->addAnimation("walk_up", 12, {0, 0}, {25, 28}, true);
    animations->addAnimation("walk_left", 10, {0, 0}, {20, 27}, true);
    animations->addAnimation("walk_right", 10, {0, 0}, {20, 27}, true);
}

void Character::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
            case sf::Keyboard::D: movingRight = true; break;
            case sf::Keyboard::A: movingLeft = true; break;
            case sf::Keyboard::W: movingUp = true; break;
            case sf::Keyboard::S: movingDown = true; break;
        }
    } else if (event.type == sf::Event::KeyReleased) {
        switch (event.key.code) {
            case sf::Keyboard::D: movingRight = false; break;
            case sf::Keyboard::A: movingLeft = false; break;
            case sf::Keyboard::W: movingUp = false; break;
            case sf::Keyboard::S: movingDown = false; break;
        }
    } else if (event.type == sf::Event::MouseMoved) {
        // Actualizar la posición del ratón cuando se mueve
        setMousePosition(sf::Vector2f(event.mouseMove.x, event.mouseMove.y));
    }
}

void Character::setMousePosition(const sf::Vector2f& position) {
    mousePosition = position;
    updateAimDirection();
}

void Character::updateAimDirection() {
    // Calcular la dirección desde el personaje hacia el ratón
    sf::Vector2f playerPos = sprite.getPosition();
    aimDirection = mousePosition - playerPos;
    
    // Normalizar el vector de dirección
    float length = std::sqrt(aimDirection.x * aimDirection.x + aimDirection.y * aimDirection.y);
    if (length > 0) {
        aimDirection.x /= length;
        aimDirection.y /= length;
    }
}

sf::Vector2f Character::getAimDirection() const {
    return aimDirection;
}

void Character::update(const TileMap& tilemap, float deltaTime) {
    sf::Vector2f moveDirection(0.f, 0.f);
    updateInvencibility(deltaTime);
    hurtbox->setPosition(getPosition());  // Centrado en la posición del enemigo
    updateHealthRegeneration(deltaTime); // Regenerar vida si está habilitado
    updateAimDirection(); // Actualizar la dirección de apuntado en cada frame
    
    if(isShieldActive) {
        shieldAnimation->update(deltaTime); // Actualizar la animación del escudo
    }
    
    // Detectar entrada del usuario solo si no está en dash
    if (!isDashing) {
        if (movingRight) { moveDirection.x += 1; }
        if (movingLeft) { moveDirection.x -= 1; }
        if (movingUp) { moveDirection.y -= 1; }
        if (movingDown) { moveDirection.y += 1; }

        // Normalizar el vector de movimiento en caso de movimiento diagonal
        if (moveDirection.x != 0 && moveDirection.y != 0) {
            moveDirection *= 0.7071f; // sqrt(2)/2 para mantener velocidad uniforme
        }

        // Aplicar aceleración si hay movimiento
        if (moveDirection.x != 0) {
            velocity.x += moveDirection.x * acceleration * deltaTime;
            if (velocity.x > speed) velocity.x = speed;
            if (velocity.x < -speed) velocity.x = -speed;
        } else {
            // Aplicar desaceleración si no hay entrada en X
            if (velocity.x > 0) {
                velocity.x -= deceleration * deltaTime;
                if (velocity.x < 0) velocity.x = 0;
            }
            if (velocity.x < 0) {
                velocity.x += deceleration * deltaTime;
                if (velocity.x > 0) velocity.x = 0;
            }
        }

        if (moveDirection.y != 0) {
            velocity.y += moveDirection.y * acceleration * deltaTime;
            if (velocity.y > speed) velocity.y = speed;
            if (velocity.y < -speed) velocity.y = -speed;
        } else {
            // Aplicar desaceleración si no hay entrada en Y
            if (velocity.y > 0) {
                velocity.y -= deceleration * deltaTime;
                if (velocity.y < 0) velocity.y = 0;
            }
            if (velocity.y < 0) {
                velocity.y += deceleration * deltaTime;
                if (velocity.y > 0) velocity.y = 0;
            }
        }
        
        // Actualizar la dirección a la que mira el personaje para que siempre sea la dirección de apuntado
        direction = aimDirection;
    }

    // Manejar el dash
    if (isDashing) {
        if (dashTimer.getElapsedTime().asSeconds() > dashDuration) {
            isDashing = false;
            velocity.x = 0;
            velocity.y = 0;
        }
    }

    // Comprobación de colisiones antes de mover al personaje
    sf::FloatRect nextBounds = sprite.getGlobalBounds();
    nextBounds.left += velocity.x * deltaTime;
    nextBounds.top += velocity.y * deltaTime;

    if (!tilemap.isColliding(nextBounds)) {
        sprite.move(velocity * deltaTime);
    } else {
        if (isDashing) {
            // Si hay colisión durante el dash, detener el dash
            isDashing = false;
            velocity = {0, 0};
        } else {
            velocity = {0, 0}; // Detiene el movimiento en caso de colisión
        }
    }

    // Actualizar animación basada en movimiento y dirección
    updateAnimation();
    
    // Actualizar la animación
    animations->update(deltaTime);
}

void Character::updateAnimation() {
    bool isMoving = (velocity.x != 0 || velocity.y != 0);
    
    // Determinar el estado de animación basado en la dirección y el movimiento
    AnimationState newState = currentState;
    
    if (isMoving) {
        // Si se está moviendo, determinar la dirección dominante
        if (std::abs(aimDirection.x) > std::abs(aimDirection.y)) {
            // Movimiento horizontal dominante
            if (aimDirection.x > 0) {
                newState = AnimationState::WALK_RIGHT;
            } else {
                newState = AnimationState::WALK_LEFT;
            }
        } else {
            // Movimiento vertical dominante
            if (aimDirection.y > 0) {
                newState = AnimationState::WALK_DOWN;
            } else {
                newState = AnimationState::WALK_UP;
            }
        }
    } else {
        // Si está quieto, mantener la misma dirección pero en idle
        switch (currentState) {
            case AnimationState::WALK_DOWN:
                newState = AnimationState::IDLE_DOWN;
                break;
            case AnimationState::WALK_UP:
                newState = AnimationState::IDLE_UP;
                break;
            case AnimationState::WALK_LEFT:
                newState = AnimationState::IDLE_LEFT;
                break;
            case AnimationState::WALK_RIGHT:
                newState = AnimationState::IDLE_RIGHT;
                break;
            default:
                // Si ya estaba en idle, mantener el estado actual
                break;
        }
    }
    
    // Si el estado ha cambiado, cambiar la animación
    changeAnimationState(newState);
}

void Character::changeAnimationState(AnimationState newState) {
    if (newState != currentState) {
        currentState = newState;
        
        // Cambiar textura si es necesario
        if (animationTextures.find(currentState) != animationTextures.end()) {
            if (!texture.loadFromFile(animationTextures[currentState])) {
                std::cerr << "Error: No se pudo cargar la textura para el estado " << static_cast<int>(currentState) << std::endl;
                return;
            }
            sprite.loadTexture(texture);
        }
        
        // Reproducir la animación correspondiente
        switch (currentState) {
            case AnimationState::IDLE_DOWN:
                animations->play("idle_down", 8.0f, true);
                break;
            case AnimationState::IDLE_UP:
                animations->play("idle_up", 8.0f, true);
                break;
            case AnimationState::IDLE_LEFT:
                animations->play("idle_left", 8.0f, true);
                break;
            case AnimationState::IDLE_RIGHT:
                animations->play("idle_right", 8.0f, true);
                break;
            case AnimationState::WALK_DOWN:
                animations->play("walk_down", 12.0f, true);
                break;
            case AnimationState::WALK_UP:
                animations->play("walk_up", 12.0f, true);
                break;
            case AnimationState::WALK_LEFT:
                animations->play("walk_left", 12.0f, true);
                break;
            case AnimationState::WALK_RIGHT:
                animations->play("walk_right", 12.0f, true);
                break;
        }
    }
}

void Character::draw(GameEngine& engine) {
    // Validar que el sprite tiene una textura asignada
    if (!sprite.getSprite().getTexture()) {
        std::cerr << "Error: El sprite no tiene una textura asignada" << std::endl;
        return;
    } 

    // Dibujar el personaje
    engine.drawSprite(sprite);

    drawHearts(engine); // Dibujar corazones de vida

    // Dibujar el arma equipada
    if (equippedWeapon) {
        equippedWeapon->renderOnPlayer(getPosition(), getDirection(), engine.getWindow());
    }

    if (isShieldActive) {
        shieldsprite.setPosition(sprite.getPosition().x - 32, sprite.getPosition().y - 32);
        shieldsprite.draw(engine.getWindow());
    }
}

void Character::spawnAt(const TileMap& tilemap, float x, float y) {
    // Obtiene una posición de spawn válida
    sf::Vector2f spawnPos = tilemap.getSpawnPosition(x * 16, y * 16);  //la casilla pasada por parametro multiplicada por los pixeles a los que equivale un tile (16*16)
    
    // Establece la posición del jugador
    setPosition(spawnPos.x, spawnPos.y);
    
    // Reinicia la velocidad y otros estados del jugador
    velocity = sf::Vector2f(0.f, 0.f);
    movingLeft = false;
    movingRight = false;
    movingUp = false;
    movingDown = false;
    isDashing = false;
    
    // Establece la dirección inicial del jugador (hacia abajo)
    direction = sf::Vector2f(0.0f, 1.0f);
    
    // Asegúrate de que la textura del sprite esté correctamente orientada
    changeAnimationState(AnimationState::IDLE_DOWN);
}

// WEAPON STUFF-------------------------------------------------------------------------------------------------------------------------------------------

void Character::equipWeapon() {
    equippedWeapon = weapons[equippedIndex];
}

void Character::startDash(float speed, float duration) {
    if (!isDashing) {
        std::cout << "startea el dashhhhhhh" << std::endl;
        isDashing = true;
        dashSpeed = speed;
        dashDuration = duration;
        dashTimer.restart();
        velocity.x = dashSpeed * aimDirection.x; // Se mueve en la dirección de apuntado
        velocity.y = dashSpeed * aimDirection.y;
    }
}

void Character::addWeapon(Weapon* weapon) {
    if(weapons.size() < 2){
        weapons.push_back(weapon);
        if(weapons.size() == 1) 
            equippedIndex = 0;
    }
}

void Character::switchWeapon() {
    if(weapons.size() > 1) {
        equippedIndex = 1 - equippedIndex;
    }
}

void Character::setWeapon(int index) {
    if (index >= 0 && index < weapons.size()) {
        equippedIndex = index;
    }
}

int Character::getWeaponCount() const {
    return weapons.size();
}

Weapon* Character::getEquippedWeapon() const {
    if (weapons.empty()) return nullptr;
    return weapons[equippedIndex];
}

void Character::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

sf::Vector2f Character::getDirection() const {
    return direction;
}

void Character::setDirection(float x, float y) {
    direction.x = x;
    direction.y = y;
}

void Character::addWeaponWithPosition(Weapon* weapon, sf::Vector2f originalPosition) {
    if (weapons.size() < 2) {
        weapons.push_back(weapon);
        weaponOriginalPositions.push_back(originalPosition);
        if (weapons.size() == 1) 
            equippedIndex = 0;
    } else {
        // Replace the first weapon (index 0)
        Weapon* oldWeapon = weapons[0];
        sf::Vector2f oldPos = weaponOriginalPositions[0];
        
        // Return the old weapon to ground
        oldWeapon->setPosition(oldPos.x, oldPos.y);
        
        // Update with the new weapon
        weapons[0] = weapon;
        weaponOriginalPositions[0] = originalPosition;
        
        // Make sure we're using the second weapon (index 1)
        equippedIndex = 1;
    }
}

Weapon* Character::removeFirstWeapon(sf::Vector2f& outOriginalPosition) {
    if (weapons.empty()) return nullptr;
    
    Weapon* removedWeapon = weapons[0];
    outOriginalPosition = weaponOriginalPositions[0];
    
    // Remove the first weapon
    weapons.erase(weapons.begin());
    weaponOriginalPositions.erase(weaponOriginalPositions.begin());
    
    // Adjust equipped index if needed
    if (!weapons.empty()) equippedIndex = 0;
    
    return removedWeapon;
}

// HEALTH STUFF-------------------------------------------------------------------------------------------------------------------------------------

void Character::setHealth(int health) {
    currentHealth = std::max(0, std::min(health, maxHealth)); // Asegura que la vida esté entre 0 y maxHealth
}

int Character::getHealth() const {
    return currentHealth;
}

int Character::getMaxHealth() const {
    return maxHealth;
}

void Character::takeDamage(int damage) {
    if(!isInvencible){
        if(tryDodge()) {
            std::cout << "Esquivas el ataque!" << std::endl;
            return; // Si se esquiva, no se recibe daño
        }
        setHealth(currentHealth - damage);
        isInvencible = true;
        if(invencibilityDuration == 4.0f) {
            isShieldActive = true; // Activar el escudo temporal
            shieldAnimation->play("shield", 12.0f); // Reproducir a 12 FPS
        }
    };
}

void Character::heal(int amount) {
    setHealth(currentHealth + amount);
}

void Character::hurt(int amount) {
    takeDamage(amount);
}

void Character::drawHearts(GameEngine& engine) {
    // Cargar el sprite de los corazones si no está cargado
    static SpriteFacade heartSprite("./resources/crucifix.png");

    // Posición base del primer corazón (encima del personaje)
    sf::Vector2f basePosition = sprite.getPosition();
    basePosition.y -= 50; // Ajustar la altura para que esté encima del personaje
    basePosition.x -= 45; // Centrar los corazones (ajustar según el tamaño del sprite)

    // Dibujar los 3 corazones
    for (int i = 0; i < 3; ++i) {
        sf::Vector2f position = basePosition;
        position.x += i * 30; // Separación entre corazones

        // Determinar el estado del corazón (lleno, medio lleno o vacío)
        float heartValue = currentHealth - (i * (maxHealth/3)); // Vida restante para este corazón
        if (heartValue >= (maxHealth/3)) {
            // Corazón lleno
            heartSprite.setTextureRect(sf::IntRect(0, 0, 32, 32)); // Rect completo para corazón lleno
        } else if (heartValue > 0) {
            // Medio corazón
            heartSprite.setTextureRect(sf::IntRect(0, 0, 16, 32)); // Rect para medio corazón
        } else {
            // Corazón vacío
           heartSprite.setTextureRect(sf::IntRect(32, 0, 32, 32)); // Rect para corazón vacío (ajustar según el sprite)
        }

        heartSprite.setPosition(position.x, position.y);
        heartSprite.draw(engine.getWindow());
    }
}

// INTERACTION STUFF-------------------------------------------------------------------------------------------------------------------------------

void Character::InteractionCage(TileMap& tilemap, int centerX, int centerY) {
    // Coordenadas relativas desde el centro
    tilemap.setLocalTile("deco", centerX - 2, centerY, 64); // izquierda
    tilemap.setLocalTile("deco", centerX - 1, centerY, 65); // centro
    tilemap.setLocalTile("deco", centerX,     centerY, 66); // derecha
    tilemap.setLocalTile("deco", centerX - 2, centerY + 1, 80); // izquierda abajo
    tilemap.setLocalTile("deco", centerX - 1, centerY + 1, 81); // centro abajo
    tilemap.setLocalTile("deco", centerX,     centerY + 1, 82); // derecha abajo
}

void Character::InteractionOpenDoor() {
    std::cout << "PUERTA ABIERTA (SE HAN PULSADO LOS 3 BOTONES DE SALA 1)" << std::endl;
}

void Character::interact(TileMap& tilemap) {
    sf::FloatRect playerBounds = sprite.getGlobalBounds();
    int tileId = -1;
    
    if (tilemap.isPlayerInteractingWithTile(playerBounds, tileId)) {
        // Crear la interacción correspondiente usando la fábrica
        tileId -= 1;
        auto interaction = InteractionFactory::createInteraction(tileId);
        
        if (interaction) {
            // Verificar si la interacción está disponible
            if (interaction->isAvailable(*this, tilemap)) {
                // Ejecutar la interacción
                interaction->execute(*this, tilemap);
                
                // Si es una puerta y se han cumplido los requisitos, abrirla
                if (auto doorInteraction = std::dynamic_pointer_cast<DoorInteraction>(interaction)) {
                    if (InteractionManager::getInstance()->checkDoorRequirements(tileId)) {
                        std::cout << "Se han cumplido todos los requisitos para abrir la puerta." << std::endl;
                    }
                }
            }
        } else {
            std::cout << "No hay interacción definida para el tile " << tileId << std::endl;
        }
    } else {
        std::cout << "No hay nada con lo que interactuar aquí." << std::endl;
    }
}

void Character::updateInvencibility(float deltaTime){
    if(isInvencible)
        invencibilityTimer += deltaTime;
    if(invencibilityTimer > invencibilityDuration){
        isInvencible = false;
        invencibilityTimer = 0;
        if(invencibilityDuration == 4.0f)
            isShieldActive = false; // Desactivar el escudo al finalizar la invencibilidad
    }
}

int Character::getKarma() const {
    return karmaPoints; // Asegúrate de que `karmaPoints` esté definido en la clase
}

void Character::addKarma(int amount) {
    karmaPoints += amount;
}

void Character::updateHealthRegeneration(float deltaTime) {
    if (healthRegenerationEnabled) {
        healthRegenTimer += deltaTime;

        if (healthRegenTimer >= healthRegenInterval) {
            healthRegenTimer = 0.0f;

            if (currentHealth < maxHealth) {
                currentHealth++;
                std::cout << "Vida regenerada. Vidas actuales: " << currentHealth << std::endl;
            }
        }
    }
}

bool Character::tryDodge() const {
    float randomValue = static_cast<float>(rand()) / RAND_MAX;
    return randomValue < dodgeChance; // `dodgeChance` debe estar definido en la clase
}

void Character::increaseMovementSpeed(float amount) {
    float oldSpeed = speed;
    speed += amount;
    std::cout << "Velocidad aumentada de " << oldSpeed << " a " << speed << std::endl;
}

void Character::increaseDodgeChance(float amount) {
    dodgeChance += amount;
}

void Character::increaseMaxHealth(int amount) {
    maxHealth += amount;
    currentHealth = std::min(currentHealth, static_cast<float>(maxHealth)); // Asegúrate de que la vida actual no exceda la máxima
}

void Character::enableHealthRegeneration() {
    healthRegenerationEnabled = true;
}

void Character::enableTemporalyShield(float duration) {
    invencibilityDuration = duration;
    invencibilityTimer = 0.0f; // Reiniciar el temporizador
}

sf::FloatRect Character::getBounds() const {
    return sprite.getGlobalBounds();
}