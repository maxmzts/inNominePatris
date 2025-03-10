#ifndef LANCE_H
#define LANCE_H

#include <SFML/Graphics.hpp>

#include "Weapon.h"

class Portal {
    public:
        Portal();
        Portal(const sf::Vector2f& pos);
        void setPosition(const sf::Vector2f& pos);
        const sf::Vector2f& getPosition() const;
    private:
        sf::Vector2f position;
};

class Lance : public Weapon {
    public:
        Lance();
        void attack(Character& character, std::vector<Enemy>& enemies) override;
        void useAbility(Character& character) override {}
        void useAbility(Character& character,  sf::RenderWindow& window) override;

        AbilityType getAbilityType() const override { return AbilityType::Teleport; };


    private:
        float attackRange;
        bool isPortalDropped;
        float PortalRange;
        Portal portal;
};



#endif // !LANCE_H