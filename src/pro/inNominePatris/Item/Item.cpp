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
        items.push_back(new DoubleDashItem());
    }
    else if(weapontype == ItemType::Lance){
        items.push_back(new AttackCooldownLanceItem());
        items.push_back(new AttackDamageLanceItem());
        items.push_back(new AttackHitboxLanceItem());
        items.push_back(new RevengeReturnItem());
    }
    else if(weapontype == ItemType::Bow){
        items.push_back(new AttackDamageBowItem());
        items.push_back(new ArrowSpeedItem());
        items.push_back(new ArrowCountItem());
        items.push_back(new QuickShotItem());
        items.push_back(new DecreaseAbilityCooldownBowItem());
    }
    else{
        std::cerr << "Error: Tipo de arma no reconocido." << std::endl;
        return nullptr;
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

void DoubleDashItem::applyEffect(Weapon& weapon) {
    Sword* sword = dynamic_cast<Sword*>(&weapon);
    if (sword) {
        sword->enableDoubleDash(); // Habilitar el doble dash en la espada
        std::cout << "¡Doble dash habilitado para la espada!" << std::endl;
    } else {
        std::cerr << "El ítem DoubleDash solo puede aplicarse a una espada." << std::endl;
    }
}

void DoubleDashItem::Picked() {
    isPickedUp = true;
    std::cout << "¡Has recogido el ítem DoubleDash!" << std::endl;
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

//Aumentar el tamaño de la hitbox del ataque
void AttackHitboxLanceItem::applyEffect(Weapon& weapon) {
    if (Lance* lance = dynamic_cast<Lance*>(&weapon)) {
        lance->increaseAttackHitbox(250,40); // Crear la hitbox de ataque
    }
}

void AttackHitboxLanceItem::Picked() {
    isPickedUp = true;
}

//Activar el ataque de venganza
void RevengeReturnItem::applyEffect(Weapon& weapon) {
    if (Lance* lance = dynamic_cast<Lance*>(&weapon)) {
        lance->activateRevengeReturn();
    }
}

void RevengeReturnItem::Picked() {
    isPickedUp = true;
}

//--------------Arco-------------------//

//Aumentar el daño del ataque
void AttackDamageBowItem::applyEffect(Weapon& weapon) {
    if (Bow* bow = dynamic_cast<Bow*>(&weapon)) {
        bow->increaseAttackDamage(8.f); // Aumentar el daño del ataque
    }
}

void AttackDamageBowItem::Picked() {
    isPickedUp = true;
}

//Aumentar la velocidad de la flecha
void ArrowSpeedItem::applyEffect(Weapon& weapon) {
    if (Bow* bow = dynamic_cast<Bow*>(&weapon)) {
        bow->increaseArrowSpeed(150.f); // Aumentar la velocidad de la flecha
    }
}

void ArrowSpeedItem::Picked() {
    isPickedUp = true;
}

//Aumentar la cantidad de flechas disparadas por la habilidad
void ArrowCountItem::applyEffect(Weapon& weapon) {
    if (Bow* bow = dynamic_cast<Bow*>(&weapon)) {
        bow->increaseAbilityArrowCount(2); // Aumentar la cantidad de flechas disparadas por la habilidad
    }
}

void ArrowCountItem::Picked() {
    isPickedUp = true;
}

//Tirar flechas más rápido
void QuickShotItem::applyEffect(Weapon& weapon) {
    if (Bow* bow = dynamic_cast<Bow*>(&weapon)) {
        bow->enableQuickShot(0.25f); // 25% chance to trigger quick shot
    }
}

void QuickShotItem::Picked() {
    isPickedUp = true;
}

//Disminuir el cooldown de la habilidad
void DecreaseAbilityCooldownBowItem::applyEffect(Weapon& weapon) {
    if (Bow* bow = dynamic_cast<Bow*>(&weapon)) {
        bow->decreaseAbilityCooldown(1.f); // Disminuir el cooldown de la habilidad
    }
}

void DecreaseAbilityCooldownBowItem::Picked() {
    isPickedUp = true;
}