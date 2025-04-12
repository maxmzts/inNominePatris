#include "Sword.h"
#include <iostream>
#include "Weapon.h"
// #include "Character.h" SE DEBE QUITAR
#include "Enemy.h"

Sword::Sword(GameEngine* engine) 
:   Weapon(engine),
    attackHitbox( std::make_shared<Hitbox>(sf::Vector2f(80.f, 100.f), sf::Vector2f(0.f, 0.f) )),
    attackCooldown(0.8f), 
    attackTimer(0.f), 
    dashSpeed(600.0f), 
    abilityCooldown(2.f), 
    lastAbilityTime(-abilityCooldown),
    slashSpriteFacade("./resources/Horizontal_Slash_Sword.png"),
    slashAnimation(slashSpriteFacade) 
{    
    spriteFacade.loadTexture("./resources/Weapons/sword.png"); // Cargar textura usando el Façade
    spriteFacade.setOrigin(16.0f, 16.0f);           // Establecer el origen
    name = "Sword"; // Nombre del arma
    
    slashAnimation.addAnimation(
        "slash",
        5,
        sf::Vector2i(0,0),
        sf::Vector2i(64, 64),
        false
    );
    slashAnimation.setAnimationEndCallback([this](){
        isAnimating = false; // Desactivar la animación al finalizar
    });
}

/**
 * Crea la hitbox para del arma para representar el ataque en el mundo del juego.
 */
void Sword::attack(sf::Vector2f position, sf::Vector2f direction) {
    if (attackTimer <= 0.f) {
        createHitbox(position, direction); // Crear la hitbox del ataque
        attackTimer = attackCooldown;     // Reiniciar el temporizador de ataque

        // Configurar la posición inicial del slash
        sf::Vector2f slashOffset;
        float rotation = 0.f;

        if (direction.x > 0) { // Derecha
            slashOffset = sf::Vector2f(25.f, -20.f); // Desplazar hacia la derecha
            rotation = 0.f;
        } else if (direction.x < 0) { // Izquierda
            slashOffset = sf::Vector2f(-25.f, 20.f); // Desplazar hacia la izquierda
            rotation = 180.f;
        } else if (direction.y > 0) { // Abajo
            slashOffset = sf::Vector2f(20.f, 25.f); // Desplazar hacia abajo
            rotation = 90.f;
        } else if (direction.y < 0) { // Arriba
            slashOffset = sf::Vector2f(-20.f, -25.f); // Desplazar hacia arriba
            rotation = -90.f;
        }

        slashSpriteFacade.setPosition(position.x + slashOffset.x, position.y + slashOffset.y); // Ajustar la posición del slash
        slashSpriteFacade.setRotation(rotation);

        // Reproducir la animación del slash
        slashAnimation.play("slash", 12.0f, false); // 12 FPS, no en bucle
        isAnimating = true;
    }
}

void Sword::createHitbox(sf::Vector2f position, sf::Vector2f direction) {
    sf::Vector2f offset;
    sf::Vector2f size;

    if (direction.x > 0) { // Derecha
        offset = sf::Vector2f(60.f, 0.f);
        size = sf::Vector2f(80.f, 50.f); // Hitbox horizontal
    } else if (direction.x < 0) { // Izquierda
        offset = sf::Vector2f(-60.f, 0.f);
        size = sf::Vector2f(80.f, 50.f); // Hitbox horizontal
    } else if (direction.y > 0) { // Abajo
        offset = sf::Vector2f(0.f, 60.f);
        size = sf::Vector2f(50.f, 80.f); // Hitbox vertical
    } else if (direction.y < 0) { // Arriba
        offset = sf::Vector2f(0.f, -60.f);
        size = sf::Vector2f(50.f, 80.f); // Hitbox vertical
    }

    attackHitbox->setSize(size); // Ajustar el tamaño de la hitbox
    attackHitbox->setPosition(position + offset); // Ajustar la posición de la hitbox
    attackHitbox->setActive(true); // Activar la hitbox
}

bool Sword::useAbility() {
    static sf::Clock clock;
    float elapsedTime = clock.getElapsedTime().asSeconds();
    if(elapsedTime - lastAbilityTime < abilityCooldown) {
        std::cout << "Ability on cooldown!" << std::endl;
        return false;
    }

    lastAbilityTime = elapsedTime;
    std::cout << "Sword ability!" << std::endl;
    return true;
}

/**
 * Ajusta el espada en la posicion y con la direccion del jugador.
 * Luego llama a render para dibujar el espada.
 */
void Sword::renderOnPlayer(sf::Vector2f position, sf::Vector2f direction) {
    // Ajustar la posición del arma en función de la dirección
    if (direction.x > 0) {  // Mirando a la derecha
        spriteFacade.setPosition(position.x + 20, position.y);
        spriteFacade.setRotation(0); // Sin rotación
    } else if (direction.x < 0) {  // Mirando a la izquierda
        spriteFacade.setPosition(position.x - 20, position.y);
        spriteFacade.setRotation(180); // Rotar 180 grados
    } else if (direction.y < 0) {  // Mirando hacia arriba
        spriteFacade.setPosition(position.x, position.y - 20);
        spriteFacade.setRotation(270); // Rotar 270 grados
    } else if (direction.y > 0) {  // Mirando hacia abajo
        spriteFacade.setPosition(position.x, position.y + 20);
        spriteFacade.setRotation(90); // Rotar 90 grados
    }

    render();
}

/**
 * Dibuja el espada en la posicion y con la direccion que tiene su sprite por defecto.
 */
void Sword::render(){
    // Dibujar el sprite del arma
    spriteFacade.draw(engine->getWindow());
    //attackHitbox.render(engine->getWindow()); // Dibujar la hitbox de ataque
    if(isAnimating) {
        slashSpriteFacade.draw(engine->getWindow()); // Dibujar la animación de ataque
    }
}

void Sword::update(float deltaTime) {
    // Actualizar el temporizador de ataque
    if (attackTimer > 0.f) {
        attackTimer -= deltaTime;
        if(attackTimer < 0.f) {
            attackHitbox->setActive(false); // Desactivar la hitbox después de un tiempo
            dealtDamage = false;
        }
    }
    // Actualizar el temporizador de habilidad
    if (lastAbilityTime > 0.f) {
        lastAbilityTime -= deltaTime;
        if(lastAbilityTime < 0.f) {
            lastAbilityTime = -abilityCooldown; // Reiniciar el temporizador de habilidad
        }
    }

    if(isAnimating){
        slashAnimation.update(deltaTime); // Actualizar la animación
    }
}

std::shared_ptr<Hitbox> Sword::getAttackHitbox() const {
    return attackHitbox;
}

void Sword::increaseDashSpeed(float speed) {
    std::cout << "Dash speed increased!" << std::endl;
    dashSpeed += speed;
}

void Sword::decreaseDashCooldown(float cooldown) {
    std::cout << "Dash cooldown decreased!" << std::endl;
    abilityCooldown -= cooldown;
}

void Sword::decreaseAttackCooldown(float cooldown) {
    std::cout << "Attack cooldown decreased!" << std::endl;
    attackCooldown -= cooldown;
}

void Sword::increaseAttackDamage(float damage) {
    std::cout << "Attack damage increased!" << std::endl;
    attackDamage += damage;
}