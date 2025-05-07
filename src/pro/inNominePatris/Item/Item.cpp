#include "Item.h"

void Item::setTexture(const std::string& texturePath) {
    spriteFacade.loadTexture(texturePath);
}

void Item::setPosition(float x, float y) {
    spriteFacade.setPosition(x, y);
}

void Item::render(sf::RenderWindow& window) {
    if (!isPickedUp) {
        spriteFacade.draw(window);
    }
}

sf::FloatRect Item::getBounds() const {
    return spriteFacade.getGlobalBounds();
}

Item* Item::generateRandomItemforWeapon(ItemType weapontype) {
    std::vector<Item*> items;

    if(weapontype == ItemType::Sword){
        items.push_back(new DashBoostItem());
        items.push_back(new DashCooldownItem());
        items.push_back(new AttackCooldownSwordItem());
        items.push_back(new AttackDamageSwordItem());
    }
    else if(weapontype == ItemType::Lance){
        items.push_back(new AttackCooldownLanceItem());
        items.push_back(new AttackDamageLanceItem());
    }
    else if(weapontype == ItemType::Bow){
    }
    
    if(items.empty()) return nullptr;

    int randomIndex = rand() % items.size();
    return items[randomIndex];
}

//--------------Espada-------------------//

//Aumentar la velocidad del dash
void DashBoostItem::applyEffect(Weapon& weapon) {
    if (Sword* sword = dynamic_cast<Sword*>(&weapon)) {
        sword->increaseDashSpeed(80.f);
    }
}

void DashBoostItem::Picked() {
    isPickedUp = true;
}

//Disminuir el cooldown del dash
void DashCooldownItem::applyEffect(Weapon& weapon) {
    if (Sword* sword = dynamic_cast<Sword*>(&weapon)) {
        sword->decreaseDashCooldown(0.5f); // Disminuir el cooldown del dash
    }
}

void DashCooldownItem::Picked() {
    isPickedUp = true;
}

//Disminuir el cooldown del ataque
void AttackCooldownSwordItem::applyEffect(Weapon& weapon) {
    if (Sword* sword = dynamic_cast<Sword*>(&weapon)) {
        sword->decreaseAttackCooldown(0.2f); // Disminuir el cooldown del ataque
    }
}

void AttackCooldownSwordItem::Picked() {
    isPickedUp = true;
}

//Aumentar el daño del ataque
void AttackDamageSwordItem::applyEffect(Weapon& weapon) {
    if (Sword* sword = dynamic_cast<Sword*>(&weapon)) {
        sword->increaseAttackDamage(10.f); // Aumentar el daño del ataque
    }
}

void AttackDamageSwordItem::Picked() {
    isPickedUp = true;
}

//--------------Lanza-------------------//

//Disminuir el cooldown del ataque
void AttackCooldownLanceItem::applyEffect(Weapon& weapon) {
    if (Lance* lance = dynamic_cast<Lance*>(&weapon)) {
        lance->decreaseAttackCooldown(0.2f); // Disminuir el cooldown del ataque
    }
}

void AttackCooldownLanceItem::Picked() {
    isPickedUp = true;
}

//Aumentar el daño del ataque
void AttackDamageLanceItem::applyEffect(Weapon& weapon) {
    if (Lance* lance = dynamic_cast<Lance*>(&weapon)) {
        lance->increaseAttackDamage(10.f); // Aumentar el daño del ataque
    }
}

void AttackDamageLanceItem::Picked() {
    isPickedUp = true;
}