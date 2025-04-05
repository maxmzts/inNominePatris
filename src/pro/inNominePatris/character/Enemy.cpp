#include "Enemy.h"
#include <iostream>

Enemy::Enemy(float x, float y) : health(100) {
    sprite.setPosition(x, y);
}

void Enemy::setTexture(const sf::Texture& tex) {
    texture = tex;  
    sprite.setTexture(texture);
}

void Enemy::setTextureRect(int left, int top, int width, int height) {
    sprite.setTextureRect(sf::IntRect(left, top, width, height));
}

void Enemy::takeDamage(int amount) {
    health -= amount;
    std::cout << "Enemigo golpeado! Vida restante: " << health << std::endl;
}

sf::FloatRect Enemy::getBounds() const {
    return sprite.getGlobalBounds();
}

void Enemy::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}
