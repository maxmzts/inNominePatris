#include "Character.h"

Character::Character() 
    : direction(0.0f, 1.0f), equippedWeapon(nullptr), isDashing(false), dashSpeed(0.0f), dashDuration(0.0f) {
        sprite.setPosition(100, 300);

    }

void Character::equipWeapon(Weapon* newWeapon) {
    equippedWeapon = newWeapon;
}

// Asigna una textura al sprite del personaje
void Character::setTexture(const sf::Texture& tex) {
    texture = tex; 
    sprite.setTexture(texture);
}

// Cambia el rectángulo de la textura
void Character::setTextureRect(int left, int top, int width, int height) {
    sprite.setTextureRect(sf::IntRect(left, top, width, height));
}

void Character::setScale(float x, float y) {
    sprite.setScale(x, y);
}

sf::Vector2f Character::getScale() const{
    return sprite.getScale();
}

void Character::setOrigin(float x, float y) {
    sprite.setOrigin(x, y);
}

void Character::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

void Character::move(float x, float y) {
    sprite.move(x, y);
}

void Character::setDirection(float x, float y) {
    direction.x = x;
    direction.y = y;
}

sf::Vector2f Character::getDirection() const {
    return direction;
}

void Character::attack(std::vector<Enemy>& enemies) {
    if (equippedWeapon) equippedWeapon->attack(*this, enemies);
}

void Character::useAbility(sf::RenderWindow& window) {
    if (equippedWeapon) {
        if (equippedWeapon->getAbilityType() == AbilityType::Dash)
            equippedWeapon->useAbility(*this);
        else if (equippedWeapon->getAbilityType() == AbilityType::Teleport)  
            equippedWeapon->useAbility(*this, window);
    }
}

void Character::startDash(float speed, float duration) {
    if (!isDashing) {
        isDashing = true;
        dashSpeed = speed;
        dashDuration = duration;
        dashTimer.restart();
        velocity.x = dashSpeed * direction.x; // Se mueve en la dirección que mira el personaje
        velocity.y = dashSpeed * direction.y;
    }
}

void Character::increaseSpeed(float speed) {
    this->speed += speed;
}

void Character::update(float deltaTime) {
    if (isDashing) {
        if (dashTimer.getElapsedTime().asSeconds() > dashDuration) {
            isDashing = false;
            velocity.x = 0;
            velocity.y = 0;
        }
    }
    sprite.move(velocity * deltaTime);
}

void Character::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::Vector2f Character::getPosition() const {
    return sprite.getPosition();
}

sf::FloatRect Character::getBounds() const {
    return sprite.getGlobalBounds();
}