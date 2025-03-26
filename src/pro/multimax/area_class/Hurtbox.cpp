#include "Hurtbox.h"

Hurtbox::Hurtbox(Entity* entityOwner, sf::Shape* shape) : DetectedArea(shape), owner(entityOwner), damageMultiplier(1.0f) {}

Entity* Hurtbox::getOwner() const {
    return owner;
}

void Hurtbox::setDamageMultiplier(float multiplier) {
    damageMultiplier = multiplier;
}

float Hurtbox::getDamageMultiplier() const {
    return damageMultiplier;
}