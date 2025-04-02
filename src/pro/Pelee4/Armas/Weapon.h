#ifndef WEAPON_H
#define WEAPON_H
#include <vector>

class Character;
class Enemy;

#include "AbilityType.h"

class Weapon {
    public:
        virtual ~Weapon() = default;
        virtual void attack(Character& character, std::vector<Enemy>& enemies) = 0;
        virtual void useAbility(Character& character) = 0;
        virtual void useAbility(Character& character, sf::RenderWindow& window) { useAbility(character); }
        virtual void useAbility(Character& character, std::vector<Enemy>& enemies) {}
        virtual AbilityType getAbilityType() const = 0;
    };

#endif // !WEAPON_H