#include "Entity.h"
/*
#include <vector>
#include <algorithm>

Entity::Entity() : health(100), isInvulnerable(false) {
    // Inicialización básica
}

Entity::~Entity() {
    // Liberar memoria de los hurtboxes
    for (auto hurtbox : hurtboxes) {
        delete hurtbox;
    }
    hurtboxes.clear();
    
    // Liberar memoria de los hitboxes
    for (auto hitbox : hitboxes) {
        delete hitbox;
    }
    hitboxes.clear();
}

void Entity::receiveDamage(float amount, Entity* attacker) {
    // Verificar si la entidad está en estado de invulnerabilidad
    if (isInvulnerable) {
        return;
    }
    
    // Aplicar el daño a la salud
    health -= static_cast<int>(amount);
    
    // Asegurarse de que la salud no sea negativa
    if (health < 0) {
        health = 0;
    }
    
    // Aquí podrías agregar lógica adicional:
    // - Efectos visuales de daño
    // - Sonidos
    // - Activar invulnerabilidad temporal
    // - Verificar si la entidad ha muerto
    if (health <= 0) {
        // Lógica de muerte
    }
}

void Entity::activateAttack(int attackIndex) {
    // Verificar que el índice sea válido
    if (attackIndex >= 0 && attackIndex < hitboxes.size()) {
        // Ejecutar el ataque
        hitboxes[attackIndex]->executeAttack();
    }
}

void Entity::addHurtbox(sf::Shape* shape) {
    // Crear una nueva Hurtbox y agregarla al vector
    Hurtbox* newHurtbox = new Hurtbox(this, shape);
    hurtboxes.push_back(newHurtbox);
}

void Entity::addHitbox(float damage) {
    // Crear un nuevo Hitbox y agregarlo al vector
    Hitbox* newHitbox = new Hitbox(this, damage);
    hitboxes.push_back(newHitbox);
}

int Entity::getHealth() const {
    return health;
}

void Entity::setHealth(int newHealth) {
    health = newHealth;
    if (health < 0) {
        health = 0;
    }
}

bool Entity::getInvulnerability() const {
    return isInvulnerable;
}

void Entity::setInvulnerability(bool invulnerable) {
    isInvulnerable = invulnerable;
}

// Métodos adicionales que podrían ser útiles

// Actualizar todos los hitboxes y hurtboxes (posición, rotación, etc.)
void Entity::updateHitboxesAndHurtboxes(const sf::Vector2f& position, float rotation) {
    // Actualizar posiciones y rotaciones de todos los hitboxes
    for (auto hitbox : hitboxes) {
        // Aquí deberías actualizar la posición y rotación del shape del hitbox
        // Por ejemplo:
        // hitbox->getShape()->setPosition(position);
        // hitbox->getShape()->setRotation(rotation);
    }
    
    // Actualizar posiciones y rotaciones de todos los hurtboxes
    for (auto hurtbox : hurtboxes) {
        // Actualizar la posición y rotación del shape del hurtbox
        // Por ejemplo:
        // hurtbox->getShape()->setPosition(position);
        // hurtbox->getShape()->setRotation(rotation);
    }
}

// Activar/desactivar hitboxes específicos (útil para animaciones de ataques)
void Entity::setHitboxActive(int hitboxIndex, bool active) {
    if (hitboxIndex >= 0 && hitboxIndex < hitboxes.size()) {
        // Asumiendo que DetectingArea tiene un método setActive
        // hitboxes[hitboxIndex]->setActive(active);
    }
}

// Método para limpiar hitboxes y hurtboxes sin destruir la entidad
void Entity::clearCombatAreas() {
    for (auto hurtbox : hurtboxes) {
        delete hurtbox;
    }
    hurtboxes.clear();
    
    for (auto hitbox : hitboxes) {
        delete hitbox;
    }
    hitboxes.clear();
}*/