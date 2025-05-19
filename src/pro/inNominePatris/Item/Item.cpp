#include "Item.h"
#include <cmath>

void Item::setTexture(const std::string& texturePath) {
    spriteFacade.loadTexture(texturePath);
}

void Item::setPosition(float x, float y) {
    spriteFacade.setPosition(x, y);
}


// Modificar el método render para mostrar también el texto:
void Item::render(sf::RenderWindow& window) {
    if (!isPickedUp) {
        // Añadimos un efecto de levitación suave para el ítem
        static float totalTime = 0.0f;
        static sf::Clock clock;
        totalTime += clock.restart().asSeconds();
        
        // Movimiento vertical suave (levitación)
        float offsetY = sin(totalTime * 2.0f) * 5.0f;
        sf::Vector2f basePos = spriteFacade.getPosition();
        spriteFacade.setPosition(basePos.x, basePos.y + offsetY);
        
        // Dibujamos el ítem con un ligero efecto de rotación
        spriteFacade.setRotation(sin(totalTime * 1.5f) * 5.0f);
        spriteFacade.draw(window);
        
        // Restauramos la posición base para el texto
        spriteFacade.setPosition(basePos.x, basePos.y);
        
        // Actualizar posición del texto para que siga al objeto
        itemNameText.setPosition(basePos.x, basePos.y - 30.0f); // Colocar el texto más arriba del objeto
        
        // Efecto de parpadeo suave para el texto
        // Oscilación de color entre blanco y un tono dorado más brillante
        float alpha = (sin(totalTime * 2.0f) + 1.0f) / 2.0f; // Valor entre 0 y 1
        sf::Color textColor(
            255, 
            210 + static_cast<int>(45 * alpha), 
            60 + static_cast<int>(140 * alpha), 
            255
        );
        
        itemNameText.setFillColor(textColor);
        
        // Crear un efecto de resplandor para texto pixelado
        // Este efecto es más visible en fuentes como Impact
        
        // Dibujar el texto
        window.draw(itemNameText);
        
        // Efecto de partículas brillantes alrededor del ítem (simulado con pequeños círculos)
        if (rand() % 5 == 0) { // Controlar la densidad de partículas
            sf::CircleShape particle;
            particle.setRadius(1.0f + (rand() % 2));
            
            // Color dorado brillante para las partículas
            sf::Color particleColor(
                255, 
                220 + rand() % 35, 
                100 + rand() % 100,
                150 + rand() % 105
            );
            
            particle.setFillColor(particleColor);
            
            // Posición aleatoria alrededor del ítem
            float angle = static_cast<float>(rand() % 360) * 3.14159f / 180.0f;
            float distance = 20.0f + static_cast<float>(rand() % 15);
            float particleX = basePos.x + cos(angle) * distance;
            float particleY = basePos.y + sin(angle) * distance;
            
            particle.setPosition(particleX, particleY);
            window.draw(particle);
        }
    }
}

sf::FloatRect Item::getBounds() const {
    return spriteFacade.getGlobalBounds();
}

void Item::setupItemName(const std::string& name) {
    itemName = name;
    itemNameText.setString(name);
    
    // Aumentar el tamaño de la fuente para mejor visibilidad
    itemNameText.setCharacterSize(16);
    
    // Hacer el texto más brillante y con mayor contraste
    itemNameText.setOutlineThickness(2.0f);
    itemNameText.setOutlineColor(sf::Color(0, 0, 0, 255)); // Negro sólido para el contorno
    
    // Centrar el texto
    sf::FloatRect textBounds = itemNameText.getLocalBounds();
    itemNameText.setOrigin(textBounds.width / 2.0f, textBounds.height + 5.0f);
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

//Revertir el aumento de velocidad del dash
void DashBoostItem::revertEffect(Weapon& weapon) {
    if (Sword* sword = dynamic_cast<Sword*>(&weapon)) {
        sword->increaseDashSpeed(-80.f); // Reducir en la misma cantidad que se aumentó
    }
}

//Disminuir el cooldown del dash
void DashCooldownItem::applyEffect(Weapon& weapon) {
    if (Sword* sword = dynamic_cast<Sword*>(&weapon)) {
        sword->decreaseDashCooldown(0.5f); // Disminuir el cooldown del dash
    }
}

//Revertir la disminución del cooldown del dash
void DashCooldownItem::revertEffect(Weapon& weapon) {
    if (Sword* sword = dynamic_cast<Sword*>(&weapon)) {
        sword->decreaseDashCooldown(-0.5f); // Aumentar en la misma cantidad que se disminuyó
    }
}

//Disminuir el cooldown del ataque
void AttackCooldownSwordItem::applyEffect(Weapon& weapon) {
    if (Sword* sword = dynamic_cast<Sword*>(&weapon)) {
        sword->decreaseAttackCooldown(0.2f); // Disminuir el cooldown del ataque
    }
}

//Revertir la disminución del cooldown del ataque
void AttackCooldownSwordItem::revertEffect(Weapon& weapon) {
    if (Sword* sword = dynamic_cast<Sword*>(&weapon)) {
        sword->decreaseAttackCooldown(-0.2f); // Aumentar en la misma cantidad que se disminuyó
    }
}

//Aumentar el daño del ataque
void AttackDamageSwordItem::applyEffect(Weapon& weapon) {
    if (Sword* sword = dynamic_cast<Sword*>(&weapon)) {
        sword->increaseAttackDamage(10.f); // Aumentar el daño del ataque
    }
}

//Revertir el aumento del daño del ataque
void AttackDamageSwordItem::revertEffect(Weapon& weapon) {
    if (Sword* sword = dynamic_cast<Sword*>(&weapon)) {
        sword->increaseAttackDamage(-10.f); // Disminuir en la misma cantidad que se aumentó
    }
}

//Habilitar el doble dash
void DoubleDashItem::applyEffect(Weapon& weapon) {
    Sword* sword = dynamic_cast<Sword*>(&weapon);
    if (sword) {
        sword->enableDoubleDash(); // Habilitar el doble dash en la espada
      // std::cout << "¡Doble dash habilitado para la espada!" << std::endl;
    } else {
        std::cerr << "El ítem DoubleDash solo puede aplicarse a una espada." << std::endl;
    }
}

//Deshabilitar el doble dash
void DoubleDashItem::revertEffect(Weapon& weapon) {
    Sword* sword = dynamic_cast<Sword*>(&weapon);
    if (sword) {
        sword->disableDoubleDash(); // Deshabilitar el doble dash en la espada
      // std::cout << "Doble dash deshabilitado para la espada." << std::endl;
    }
}

//--------------Lanza-------------------//

//Disminuir el cooldown del ataque
void AttackCooldownLanceItem::applyEffect(Weapon& weapon) {
    if (Lance* lance = dynamic_cast<Lance*>(&weapon)) {
        lance->decreaseAttackCooldown(0.2f); // Disminuir el cooldown del ataque
    }
}

//Revertir la disminución del cooldown del ataque
void AttackCooldownLanceItem::revertEffect(Weapon& weapon) {
    if (Lance* lance = dynamic_cast<Lance*>(&weapon)) {
        lance->decreaseAttackCooldown(-0.2f); // Aumentar en la misma cantidad que se disminuyó
    }
}

//Aumentar el daño del ataque
void AttackDamageLanceItem::applyEffect(Weapon& weapon) {
    if (Lance* lance = dynamic_cast<Lance*>(&weapon)) {
        lance->increaseAttackDamage(10.f); // Aumentar el daño del ataque
    }
}

//Revertir el aumento del daño del ataque
void AttackDamageLanceItem::revertEffect(Weapon& weapon) {
    if (Lance* lance = dynamic_cast<Lance*>(&weapon)) {
        lance->increaseAttackDamage(-10.f); // Disminuir en la misma cantidad que se aumentó
    }
}

//Aumentar el tamaño de la hitbox del ataque
void AttackHitboxLanceItem::applyEffect(Weapon& weapon) {
    if (Lance* lance = dynamic_cast<Lance*>(&weapon)) {
        lance->increaseAttackHitbox(250, 40); // Crear la hitbox de ataque
    }
}

//Revertir el aumento del tamaño de la hitbox del ataque
void AttackHitboxLanceItem::revertEffect(Weapon& weapon) {
    if (Lance* lance = dynamic_cast<Lance*>(&weapon)) {
        lance->increaseAttackHitbox(-250, -40); // Reducir la hitbox de ataque
    }
}

//Activar el ataque de venganza
void RevengeReturnItem::applyEffect(Weapon& weapon) {
    if (Lance* lance = dynamic_cast<Lance*>(&weapon)) {
        lance->activateRevengeReturn();
    }
}

//Desactivar el ataque de venganza
void RevengeReturnItem::revertEffect(Weapon& weapon) {
    if (Lance* lance = dynamic_cast<Lance*>(&weapon)) {
        lance->deactivateRevengeReturn();
    }
}

//--------------Arco-------------------//

//Aumentar el daño del ataque
void AttackDamageBowItem::applyEffect(Weapon& weapon) {
    if (Bow* bow = dynamic_cast<Bow*>(&weapon)) {
        bow->increaseAttackDamage(8.f); // Aumentar el daño del ataque
    }
}

//Revertir el aumento del daño del ataque
void AttackDamageBowItem::revertEffect(Weapon& weapon) {
    if (Bow* bow = dynamic_cast<Bow*>(&weapon)) {
        bow->increaseAttackDamage(-8.f); // Disminuir en la misma cantidad que se aumentó
    }
}

//Aumentar la velocidad de la flecha
void ArrowSpeedItem::applyEffect(Weapon& weapon) {
    if (Bow* bow = dynamic_cast<Bow*>(&weapon)) {
        bow->increaseArrowSpeed(150.f); // Aumentar la velocidad de la flecha
    }
}

//Revertir el aumento de la velocidad de la flecha
void ArrowSpeedItem::revertEffect(Weapon& weapon) {
    if (Bow* bow = dynamic_cast<Bow*>(&weapon)) {
        bow->increaseArrowSpeed(-150.f); // Disminuir en la misma cantidad que se aumentó
    }
}

//Aumentar la cantidad de flechas disparadas por la habilidad
void ArrowCountItem::applyEffect(Weapon& weapon) {
    if (Bow* bow = dynamic_cast<Bow*>(&weapon)) {
        bow->increaseAbilityArrowCount(2); // Aumentar la cantidad de flechas disparadas por la habilidad
    }
}

//Revertir el aumento de la cantidad de flechas disparadas por la habilidad
void ArrowCountItem::revertEffect(Weapon& weapon) {
    if (Bow* bow = dynamic_cast<Bow*>(&weapon)) {
        bow->increaseAbilityArrowCount(-2); // Disminuir en la misma cantidad que se aumentó
    }
}

//Tirar flechas más rápido
void QuickShotItem::applyEffect(Weapon& weapon) {
    if (Bow* bow = dynamic_cast<Bow*>(&weapon)) {
        bow->enableQuickShot(0.25f); // 25% chance to trigger quick shot
    }
}

//Deshabilitar el disparo rápido
void QuickShotItem::revertEffect(Weapon& weapon) {
    if (Bow* bow = dynamic_cast<Bow*>(&weapon)) {
        bow->disableQuickShot(); // Deshabilitar el disparo rápido
    }
}

//Disminuir el cooldown de la habilidad
void DecreaseAbilityCooldownBowItem::applyEffect(Weapon& weapon) {
    if (Bow* bow = dynamic_cast<Bow*>(&weapon)) {
        bow->decreaseAbilityCooldown(1.f); // Disminuir el cooldown de la habilidad
    }
}

//Revertir la disminución del cooldown de la habilidad
void DecreaseAbilityCooldownBowItem::revertEffect(Weapon& weapon) {
    if (Bow* bow = dynamic_cast<Bow*>(&weapon)) {
        bow->decreaseAbilityCooldown(-1.f); // Aumentar en la misma cantidad que se disminuyó
    }
}