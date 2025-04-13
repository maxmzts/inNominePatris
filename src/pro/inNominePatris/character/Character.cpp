#include "Character.h"
#include "InteractionFactory.h"
#include <iostream>

Character::Character(const std::string& textureFile) 
: speed(200.f), acceleration(800.f), deceleration(1000.f), equippedWeapon(nullptr), 
isDashing(false), dashSpeed(400.f), dashDuration(0.2f), weapons(), equippedIndex(0), 
direction(0.f, 0.f), maxHealth(100), currentHealth(100), isInvencible(false) {
    
    if (!texture.loadFromFile(textureFile)) {
        std::cerr << "Error cargando la textura" << std::endl;
        exit(1);
    }
    
    sprite.setTexture(texture);
    sprite.setOrigin(75 / 2, 75 / 2);
    sprite.setTextureRect(sf::IntRect(0, 0, 75, 75));
    sprite.setPosition(320, 240);
    sprite.setScale(0.6f, 0.6f);

    velocity = sf::Vector2f(0.f, 0.f);

    hurtbox = new Hurtbox(sf::Vector2f(60.0f, 60.0f), sf::Vector2f(0.0f, 0.0f));
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
    }
}

void Character::update(const TileMap& tilemap, float deltaTime) {
    sf::Vector2f moveDirection(0.f, 0.f);
    updateInvencibility(deltaTime);
    hurtbox->setPosition(getPosition());  // Centrado en la posición del enemigo
    updateHealthRegeneration(deltaTime); // Regenerar vida si está habilitado;
    // Detectar entrada del usuario solo si no está en dash
    if (!isDashing) {
        if (movingRight) { moveDirection.x += 1; setDirection(1.0f, 0.0f); }
        if (movingLeft) { moveDirection.x -= 1; setDirection(-1.0f, 0.0f); }
        if (movingUp) { moveDirection.y -= 1; setDirection(0.0f, -1.0f); }
        if (movingDown) { moveDirection.y += 1; setDirection(0.0f, 1.0f); }

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
    }

    // Manejar el dash
    if (isDashing) {
        if (dashTimer.getElapsedTime().asSeconds() > dashDuration) {
            isDashing = false;
            velocity.x = 0;
            velocity.y = 0;
        }
    }

    // **Comprobación de colisiones antes de mover al personaje**
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

    // Ajustar la textura según la dirección del movimiento
    if (movingRight) {
        sprite.setTextureRect(sf::IntRect(0, 2 * 75, 75, 75));
        sprite.setScale(0.6f, 0.6f);
    }
    if (movingLeft) {
        sprite.setTextureRect(sf::IntRect(0, 2 * 75, 75, 75));
        sprite.setScale(-0.6f, 0.6f);
    }
    if (movingUp) sprite.setTextureRect(sf::IntRect(0, 3 * 75, 75, 75));
    if (movingDown) sprite.setTextureRect(sf::IntRect(0, 0 * 75, 75, 75));
}

void Character::draw(GameEngine& engine) {
    /////// DEBUG
    hurtbox->render(engine.getWindow());
    
    // Dibujar el personaje
    engine.drawSprite(sprite);

    // Dibujar el arma equipada
    if (equippedWeapon) {
        equippedWeapon->renderOnPlayer(getPosition(), getDirection()); // Pasar el posicion y direccion para ajustar la posición del arma
    }

    // Dibujar barra de vida sobre el personaje
    sf::RectangleShape healthBar(sf::Vector2f(50, 5));
    healthBar.setFillColor(sf::Color::Green);
    healthBar.setPosition(sprite.getPosition().x - 25, sprite.getPosition().y - 40);

    float healthPercentage = static_cast<float>(currentHealth) / maxHealth;
    healthBar.setSize(sf::Vector2f(50 * healthPercentage, 5));

    engine.drawRectangle(healthBar);
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
    sprite.setTextureRect(sf::IntRect(0, 0, 75, 75));
}












// WEAPON STUFF-------------------------------------------------------------------------------------------------------------------------------------------

void Character::equipWeapon() {
    equippedWeapon = weapons[equippedIndex];
}

// ESTO HAY QUE CAMBIARLO
// lo que debería hacer es activar la hitbox y dejar que el InGame gestione las intersecciones
// crea una referencia circular al enemigo 
//
// void Character::attack(std::vector<Enemy>& enemies) {
//     if (equippedWeapon) equippedWeapon->attack(*this, enemies);
// }

// void Character::useAbility(sf::RenderWindow& window, std::vector<Enemy>& enemies) {
//     if (equippedWeapon) {
//         if (equippedWeapon->getAbilityType() == AbilityType::Dash)
//             equippedWeapon->useAbility(*this);
//         else if (equippedWeapon->getAbilityType() == AbilityType::Teleport)  
//             equippedWeapon->useAbility(*this, window);
//         else if (equippedWeapon->getAbilityType() == AbilityType::Shot)
//             equippedWeapon->useAbility(*this, enemies);
//     }
// }

void Character::startDash(float speed, float duration) {
    if (!isDashing) {
        std::cout << "startea el dashhhhhhh" << std::endl;
        isDashing = true;
        dashSpeed = speed;
        dashDuration = duration;
        dashTimer.restart();
        velocity.x = dashSpeed * direction.x; // Se mueve en la dirección que mira el personaje
        velocity.y = dashSpeed * direction.y;
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
    };
}

void Character::heal(int amount) {
    setHealth(currentHealth + amount);
}









// INTERACTION STUFF-------------------------------------------------------------------------------------------------------------------------------

// void Character::InteractionCage(TileMap& tilemap, int centerX, int centerY) {
//     // Coordenadas relativas desde el centro
//     tilemap.setLocalTile("deco", centerX - 2, centerY, 64); // izquierda
//     tilemap.setLocalTile("deco", centerX - 1, centerY, 65); // centro
//     tilemap.setLocalTile("deco", centerX,     centerY, 66); // derecha
//     tilemap.setLocalTile("deco", centerX - 2, centerY + 1, 80); // izquierda abajo
//     tilemap.setLocalTile("deco", centerX - 1, centerY + 1, 81); // centro abajo
//     tilemap.setLocalTile("deco", centerX,     centerY + 1, 82); // derecha abajo
// }

// void Character::InteractionOpenDoor() {
//     std::cout << "PUERTA ABIERTA (SE HAN PULSADO LOS 3 BOTONES DE SALA 1)" << std::endl;
// }

void Character::interact(TileMap& tilemap) {
    sf::FloatRect playerBounds = sprite.getGlobalBounds();
    int tileId = -1;
    
    if (tilemap.isPlayerInteractingWithTile(playerBounds, tileId)) {
        // Crear la interacción correspondiente usando la fábrica
        tileId -= 1;
        auto interaction = InteractionFactory::createInteraction(tileId);
        
        if (interaction) {
            // Ejecutar la interacción
            interaction->execute(*this, tilemap);
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
    }
}
