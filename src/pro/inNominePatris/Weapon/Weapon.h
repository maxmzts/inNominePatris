#ifndef WEAPON_H
#define WEAPON_H
#include <vector>
#include "GameEngine.h"

#include "AbilityType.h"

class Weapon {
    protected:
        GameEngine* engine;
        std::string name;
    
    public:
        Weapon(GameEngine* engine) : engine(engine) {}
        virtual ~Weapon() = default;
        
        virtual void attack(sf::Vector2f position, sf::Vector2f direction) = 0;
        // REIMPLEMENTAR PORQUE CAUSA REFERENCIAS CIRCULARES POR TODOS LADOS
        // virtual void useAbility(Character& character) = 0;
        // virtual void useAbility(Character& character, sf::RenderWindow& window) { useAbility(character); }
        // virtual void useAbility(Character& character, std::vector<Enemy>& enemies) {}
        virtual AbilityType getAbilityType() const = 0;

        // Método para dibujar el arma, ajustado al personaje
        //virtual void draw(sf::Vector2f position, sf::Vector2f direction) = 0;
        virtual void render() = 0;
        virtual void renderOnPlayer(sf::Vector2f position, sf::Vector2f direction) = 0;
        virtual sf::Vector2f getPosition() const = 0;
        virtual void setPosition(float x, float y) = 0;

        // Métodos para obtener y establecer el nombre del arma
        const std::string& getName() const { return name; }
    };

#endif // !WEAPON_H