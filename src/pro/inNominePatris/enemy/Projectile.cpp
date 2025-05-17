#include "Projectile.h"
#include "Character.h"
#include "AnimatedSprite.h"
#include <cmath>
#include <SFML/Graphics.hpp>

Projectile::Projectile(sf::Vector2f position, sf::Vector2f velocity, float damage, float lifetime) 
    : position(position)
    , velocity(velocity)
    , damage(damage)
    , lifetime(lifetime)
    , elapsedTime(0.f)
{
    // Cargar la textura y configurar el sprite
    sprite.loadTexture("resources/enemies/Blood_Projectile.png");
    sf::FloatRect bounds = sprite.getSprite().getGlobalBounds();
        // Tamaño de un frame individual
    const float frameWidth = 127.f;
    const float frameHeight = 199.f;

    // Antes de crear el AnimatedSprite:
    sprite.setOrigin(frameWidth / 2.f, frameHeight / 2.f);
    sprite.setPosition(position.x, position.y);
    sprite.setScale(0.5f, 0.5f); // Escalar el sprite a la mitad de su tamaño original
    // Inicializar la animación del proyectil
    animatedSprite = new AnimatedSprite(sprite);
    
    // Configurar la animación del proyectil
    // Asumiendo que el spritesheet tiene 10 frames horizontales
    animatedSprite->addAnimation("disparoRojo", 10, sf::Vector2i(0, 0), sf::Vector2i(127.f, 199.f), true);
    animatedSprite->play("disparoRojo", 15.0f, true); // 15 fps, en bucle
    
    // Calcular la rotación basada en la dirección del movimiento
    float angle = std::atan2(velocity.y, velocity.x) * 180 / 3.14159f;
    sprite.setRotation(angle + 90.f);
    
    // Configurar el hitbox (más pequeño que el sprite visual)
    float hitboxSize = 16.f; // Tamaño del hitbox del proyectil
    hitbox = new Hitbox(sf::Vector2f(hitboxSize, hitboxSize), sf::Vector2f(-hitboxSize/2.f, -hitboxSize/2.f));
    hitbox->setPosition(position);
    hitbox->setActive(true);
}

Projectile::~Projectile() {
    delete hitbox;
    delete animatedSprite;
}

void Projectile::update(float deltaTime) {
    // Actualizar el tiempo de vida
    elapsedTime += deltaTime;
    
    // Actualizar la posición basada en la velocidad
    position += velocity * deltaTime;
    
    // Actualizar la posición del sprite y el hitbox
    sprite.setPosition(position.x, position.y);
    hitbox->setPosition(position);
    
    // Actualizar la animación del proyectil
    animatedSprite->update(deltaTime);
}

void Projectile::render(sf::RenderWindow& window) {
    // Renderizar el sprite
    sprite.draw(window);
    
    // DEBUG: Descomentar para ver el hitbox
    // hitbox->render(window);
}

bool Projectile::checkCollision(Character* player) {
    if (!player || !hitbox->isActive()) {
        return false;
    }
    // Comprobación directa usando los bounds
    return (hitbox->getGlobalBounds().intersects(player->getHurtbox()->getGlobalBounds()));
}

bool Projectile::isExpired() const {
    return elapsedTime >= lifetime;
}