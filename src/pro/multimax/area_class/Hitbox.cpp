#include "Hitbox.h"

Hitbox::Hitbox(Entity* entityOwner, float baseDamage) : DetectingArea(), owner(entityOwner), damage(baseDamage) {}

bool Hitbox::executeAttack() {
    // Buscar la Hurtbox más cercana
    Hurtbox* closestHurtbox = getClosestArea();
    
    if (closestHurtbox && closestHurtbox->getOwner() != owner) {
        // Calcular el daño final (puede incluir multiplicadores)
        float finalDamage = damage * closestHurtbox->getDamageMultiplier();
        
        // Aplicar daño a la entidad asociada a la Hurtbox
        closestHurtbox->getOwner()->receiveDamage(finalDamage, owner);
        return true;
    }
    
    return false;
}

Hurtbox* Hitbox::findClosestHurtbox() {
    // Obtener todas las áreas detectadas
    auto detectedAreas = getDetectedAreas();
    if (detectedAreas.empty()) {
        return nullptr;
    }
    
    Hurtbox* closestHurtbox = nullptr;
    float minDistance = std::numeric_limits<float>::max();
    
    // Obtener la posición del origen del Hitbox
    sf::Vector2f hitboxOrigin = getShape()->getPosition();
    
    // Buscar la Hurtbox más cercana
    for (auto area : detectedAreas) {
        // Verificar si el área detectada es una Hurtbox mediante dynamic_cast
        Hurtbox* hurtbox = dynamic_cast<Hurtbox*>(area);
        
        if (hurtbox && hurtbox->getOwner() != owner) {
            sf::Vector2f hurtboxPos = hurtbox->getShape()->getPosition();
            
            // Calcular la distancia
            float distance = std::sqrt(
                std::pow(hitboxOrigin.x - hurtboxPos.x, 2) +
                std::pow(hitboxOrigin.y - hurtboxPos.y, 2)
            );
            
            // Actualizar si es más cercana
            if (distance < minDistance) {
                minDistance = distance;
                closestHurtbox = hurtbox;
            }
        }
    }
    
    return closestHurtbox;
}

void Hitbox::setDamage(float newDamage) {
    damage = newDamage;
}

float Hitbox::getDamage() const {
    return damage;
}