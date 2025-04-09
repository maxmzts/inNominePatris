#ifndef WEAPON_H
#define WEAPON_H
#include <vector>
#include "GameEngine.h"
class Character;
class Enemy;

#include "AbilityType.h"

class Weapon {
    protected:
        GameEngine* engine;
        std::string name;
    
    public:
        Weapon(GameEngine* engine) : engine(engine) {}
        virtual ~Weapon() = default;
        
        virtual void attack(Character& character, std::vector<Enemy>& enemies) = 0;
        virtual void useAbility(Character& character) = 0;
        virtual void useAbility(Character& character, sf::RenderWindow& window) { useAbility(character); }
        virtual void useAbility(Character& character, std::vector<Enemy>& enemies) {}
        virtual AbilityType getAbilityType() const = 0;

        // Método para dibujar el arma, ajustado al personaje
        virtual void draw(GameEngine& engine, const Character* character) = 0;
        virtual sf::Vector2f getPosition() const = 0;
        virtual void setPosition(float x, float y) = 0;

        // Métodos para obtener y establecer el nombre del arma
        const std::string& getName() const { return name; }
    };

#endif // !WEAPON_H