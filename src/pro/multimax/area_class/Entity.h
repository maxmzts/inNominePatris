#pragma once

#include <vector>
#include "Hitbox.h"
#include "Hurtbox.h"

class Entity {
    private:
        std::vector<Hurtbox*> hurtboxes; // Áreas vulnerables
        std::vector<Hitbox*> hitboxes; // Áreas de ataque
        
        int health; // Salud de la entidad
        bool isInvulnerable; // Estado de invulnerabilidad
        
    public:
        // Constructor y destructor
        /*
        Entity();
        ~Entity();
        
        // Método para recibir daño
        void receiveDamage(float amount, Entity* attacker);
        
        // Método para activar un ataque específico
        void activateAttack(int attackIndex);
        
        // Métodos para añadir/eliminar hitboxes y hurtboxes
        void addHurtbox(sf::Shape* shape);
        void addHitbox(float damage);
        
        // Getters y setters para salud e invulnerabilidad
        int getHealth() const;
        void setHealth(int newHealth);
        bool getInvulnerability() const;
        void setInvulnerability(bool invulnerable);*/
};