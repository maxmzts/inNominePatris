#pragma once

#include "DetectedArea.h"

class Hurtbox : public DetectedArea {
private:
    float damageMultiplier; // Opcional: multiplicador de daño para zonas críticas

public:
    // Constructor con el puntero a la entidad propietaria
    Hurtbox(sf::Shape* shape);
    
    // Métodos para configurar y obtener el multiplicador de daño
    void setDamageMultiplier(float multiplier);
    float getDamageMultiplier() const;
};