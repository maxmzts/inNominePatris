#include "Sword.h"
#include <iostream>
#include "Weapon.h"
// #include "Character.h" SE DEBE QUITAR
#include "../hboxes/Hitbox.h"
#include "Enemy.h"
#include <cmath>

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

    baseDamage = 25.0f;
}

/**
 * Crea la hitbox para del arma para representar el ataque en el mundo del juego.
 */
void Sword::attack(sf::Vector2f position, sf::Vector2f direction) {
    if (attackTimer <= 0.f) {
        createHitbox(position, direction); // Usar la dirección de apuntado
        attackTimer = attackCooldown;     // Reiniciar el temporizador de ataque
        increaseConsecutiveAttacks();
        // Configurar la posición inicial del slash basado en la dirección de apuntado
        sf::Vector2f slashOffset;
        float rotation = 0.f;
        
        // Calcular el ángulo de rotación basado en la dirección de apuntado
        float angle = std::atan2(direction.y, direction.x) * 180 / M_PI;
        rotation = angle;
        
        // Ajustar el offset basado en el ángulo
        slashOffset.x = direction.x * 25.f;
        slashOffset.y = direction.y * 25.f;

        slashSpriteFacade.setPosition(position.x + slashOffset.x, position.y + slashOffset.y);
        slashSpriteFacade.setRotation(rotation);

        // Reproducir la animación del slash
        slashAnimation.play("slash", 12.0f, false);
        isAnimating = true;

        float damage = calculateDamage();
        std::cout << "Sword attack: Dealt " << damage << " damage!" << std::endl;
    }
}

void Sword::createHitbox(sf::Vector2f position, sf::Vector2f direction) {
    sf::Vector2f offset;
    sf::Vector2f size(80.f, 50.f); // Tamaño base de la hitbox
    
    // Calcular el ángulo para la dirección de apuntado
    float angle = std::atan2(direction.y, direction.x);
    
    // Calcular el offset basado en la dirección normalizada
    offset.x = direction.x * 60.f;
    offset.y = direction.y * 60.f;
    
    // Ajustar el tamaño de la hitbox según la dirección
    if (std::abs(direction.x) > std::abs(direction.y)) {
        // Más horizontal que vertical
        size = sf::Vector2f(80.f, 50.f);
    } else {
        // Más vertical que horizontal
        size = sf::Vector2f(50.f, 80.f);
    }

    attackHitbox->setSize(size);
    attackHitbox->setPosition(position + offset);
    attackHitbox->setActive(true);
}


bool Sword::useAbility() {
    static sf::Clock clock;
    float elapsedTime = clock.getElapsedTime().asSeconds();
    if(elapsedTime - lastAbilityTime < abilityCooldown) {
        std::cout << "Ability on cooldown!" << std::endl;
        return false;
    }

    lastAbilityTime = elapsedTime;
    consecutiveAttacks = 0;
    std::cout << "Sword ability!" << std::endl;
    return true;
}

/**
 * Ajusta el espada en la posicion y con la direccion del jugador.
 * Luego llama a render para dibujar el espada.
 */
void Sword::renderOnPlayer(sf::Vector2f position, sf::Vector2f direction) {
    // Calcular el ángulo para la dirección de apuntado
    float angle = std::atan2(direction.y, direction.x) * 180 / M_PI;
    
    // Calcular el offset basado en la dirección normalizada
    sf::Vector2f offset;
    offset.x = direction.x * 20.f;
    offset.y = direction.y * 20.f;
    
    // Posicionar la espada en la dirección de apuntado
    spriteFacade.setPosition(position.x + offset.x, position.y + offset.y);
    spriteFacade.setRotation(angle);
    
    render();
}

/**
 * Dibuja el espada en la posicion y con la direccion que tiene su sprite por defecto.
 */
// void Sword::render(){
//     // Dibujar el sprite del arma
//     spriteFacade.draw(engine->getWindow());
//     //attackHitbox.render(engine->getWindow()); // Dibujar la hitbox de ataque
//     if(isAnimating) {
//         slashSpriteFacade.draw(engine->getWindow()); // Dibujar la animación de ataque
//     }
// }

// filepath: /proyecto-abp-grupo-f4/src/pro/inNominePatris/Weapon/Sword.cpp
void Sword::render(sf::RenderWindow& window) {
    // Dibujar el sprite del arma
    spriteFacade.draw(engine->getRenderWindow());
    //attackHitbox.render(engine.getRenderWindow()); // Dibujar la hitbox de ataque
    if (isAnimating) {
        slashSpriteFacade.draw(engine->getRenderWindow()); // Dibujar la animación de ataque
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

    if(comboDamageBonus > 0.0f) {
        updateConsecutiveAttacks(deltaTime);
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
    baseDamage += damage;
}