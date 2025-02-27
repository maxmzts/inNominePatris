#include "Character.h"

Character::Character() 
    : equippedWeapon(nullptr), isDashing(false), dashSpeed(0.0f), dashDuration(0.0f) {
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

void Character::attack(std::vector<Enemy>& enemies) {
    if (equippedWeapon) equippedWeapon->attack(*this, enemies);
}

void Character::useAbility() {
    if (equippedWeapon) {
        equippedWeapon->useAbility(*this);
    }
}

void Character::startDash(float speed, float duration) {
    if (!isDashing) {
        isDashing = true;
        dashSpeed = speed;
        dashDuration = duration;
        dashTimer.restart();
        velocity.x = dashSpeed;
    }
}

void Character::update(float deltaTime) {
    if (isDashing) {
        if (dashTimer.getElapsedTime().asSeconds() > dashDuration) {
            isDashing = false;
            velocity.x = 0;
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
