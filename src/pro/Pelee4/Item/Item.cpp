#include "Item.h"
#include <iostream>
#include <random>

void AttackRangeSword::applyEffect(Weapon& weapon) {
    if(!isPickedUp){
        std::cout << "Attack Range boost applied!" << std::endl;
        Sword* sword = dynamic_cast<Sword*>(&weapon);
        sword->increaseAttackRange(20.0f);
        markAsPicked();
    }
}

void AttackRangeSword::Picked(){
    isPickedUp = true;
}

void DashBoostItem::applyEffect(Weapon& weapon) {
    if(!isPickedUp){
        std::cout << "Dash boost applied!" << std::endl;
        Sword* sword = dynamic_cast<Sword*>(&weapon);
        sword->increaseDashSpeed(500.0f);
        markAsPicked();
    }
}

void DashBoostItem::Picked(){
    isPickedUp = true;
}

void AttackRangeLance::applyEffect(Weapon& weapon) {
    if (!isPickedUp) {
        std::cout << "Attack Range boost applied!" << std::endl;
        Lance* lance = dynamic_cast<Lance*>(&weapon);
        lance->increaseAttackRange(20.0f);
        markAsPicked();
    }
}


void PortalRangeItem::applyEffect(Weapon& weapon) {
    if (!isPickedUp) {
        std::cout << "Portal Range boost applied!" << std::endl;
        Lance* lance = dynamic_cast<Lance*>(&weapon);
        lance->increasePortalRange(50.0f);
        markAsPicked();
    }
}

void PortalRangeItem::Picked(){
    isPickedUp = true;
}

void ArrowSpeedItem::applyEffect(Weapon& weapon) {
    if (!isPickedUp) {
        std::cout << "Arrow Speed boost applied!" << std::endl;
        Bow* bow = dynamic_cast<Bow*>(&weapon);
        bow->increaseArrowSpeed(50.0f);
        markAsPicked();
    }
}

void ArrowSpeedItem::Picked(){
    isPickedUp = true;
}

void AbilityArrowCountItem::applyEffect(Weapon& weapon) {
    if (!isPickedUp) {
        std::cout << "Ability Arrow Count boost applied!" << std::endl;
        Bow* bow = dynamic_cast<Bow*>(&weapon);
        bow->increaseAbilityArrowCount(1);
        markAsPicked();
    }
}

void AbilityArrowCountItem::Picked(){
    isPickedUp = true;
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

Item* Item::generateRandomItemforWeapon(ItemType weapontype) {
    std::vector<Item*> items;

    if(weapontype == ItemType::Sword){
        items.push_back(new DashBoostItem());
        items.push_back(new AttackRangeSword());
    }
    else if(weapontype == ItemType::Lance){
        items.push_back(new PortalRangeItem());
    }
    else if(weapontype == ItemType::Bow){
        items.push_back(new ArrowSpeedItem());
        items.push_back(new AbilityArrowCountItem());
    }
    
    if(items.empty()) return nullptr;

    int randomIndex = rand() % items.size();
    return items[randomIndex];
}