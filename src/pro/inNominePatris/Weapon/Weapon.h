#ifndef WEAPON_H
#define WEAPON_H
#include <vector>
#include "GameEngine.h"
#include "Hitbox.h"

#include "AbilityType.h"

class Weapon {
    protected:
        GameEngine* engine;
        std::string name;
        bool dealtDamage = false; // Indica si el arma ha causado daño en el último ataque
    
    public:
        Weapon(GameEngine* engine) : engine(engine) {}
        virtual ~Weapon() = default;
        
        virtual void attack(sf::Vector2f position, sf::Vector2f direction) = 0;
        // REIMPLEMENTAR PORQUE CAUSA REFERENCIAS CIRCULARES POR TODOS LADOS
        virtual bool useAbility() = 0;
        virtual void useAbility(sf::Vector2f characterPosition, sf::Vector2f mousePosition) = 0;
        virtual AbilityType getAbilityType() const = 0;

        // Método para dibujar el arma, ajustado al personaje
        //virtual void draw(sf::Vector2f position, sf::Vector2f direction) = 0;
        virtual void render() = 0;
        virtual void update(float deltaTime) = 0;
        virtual void renderOnPlayer(sf::Vector2f position, sf::Vector2f direction) = 0;
        virtual sf::Vector2f getPosition() const = 0;
        virtual void setPosition(float x, float y) = 0;
        virtual Hitbox getAttackHitbox() const = 0; // Método para obtener la hitbox de ataque
        bool hasDealtDamage() const { return dealtDamage; }
        void setDealtDamage(bool value) { dealtDamage = value; }
        virtual float getAttackDamage() const = 0; // Método para obtener el daño del arma


        // Métodos para obtener y establecer el nombre del arma
        const std::string& getName() const { return name; }
    };

#endif // !WEAPON_H