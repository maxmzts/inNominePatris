#include "Item.h"
#include <iostream>

void DashBoostItem::applyEffect(Sword& sword) {
    if(!isPickedUp){
        std::cout << "Dash boost applied!" << std::endl;
        sword.increaseDashSpeed(500.0f);
        isPickedUp = true;
    }
}

void SpeedBoostItem::applyEffect(Sword& sword) {
    std::cout << "Speed boost applied!" << std::endl;
}

void Item::setTexture(const sf::Texture& tex) {
    texture = tex;
    sprite.setTexture(texture);
}

void Item::setTextureRect(int left, int top, int width, int height) {
    sprite.setTextureRect(sf::IntRect(left, top, width, height));
}

void Item::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

void Item::draw(sf::RenderWindow& window) {
    if(!isPickedUp)
        window.draw(sprite);
}

sf::FloatRect Item::getBounds() const {
    return sprite.getGlobalBounds();
}