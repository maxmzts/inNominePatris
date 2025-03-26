#pragma once

#include "DetectingArea.h"
#include "Entity.h"

class Hitbox : public DetectingArea {
    private:
        float damage; // Cantidad de daño base
        Entity* owner; // Puntero a la entidad atacante
    
    public:
        // Constructor
        Hitbox(Entity* entityOwner, float baseDamage);
        
        // Método para realizar un ataque
        // Retorna true si golpeó algo, false en caso contrario
        bool executeAttack();
        
        // Método para encontrar la Hurtbox más cercana
        Hurtbox* findClosestHurtbox();
        
        // Métodos set/get para el daño
        void setDamage(float newDamage);
        float getDamage() const;
};