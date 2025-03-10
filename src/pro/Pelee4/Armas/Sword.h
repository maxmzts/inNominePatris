#ifndef SWORD_H
#define SWORD_H

#include <SFML/Graphics.hpp>

#include "Weapon.h"

class Sword : public Weapon {
    public:
        Sword();
        void attack(Character& character, std::vector<Enemy>& enemies) override;
        void useAbility(Character& character) override;
        void increaseDashSpeed(float speed);

        AbilityType getAbilityType() const override { return AbilityType::Dash; };
    private:
        float attackRange;
        float dashSpeed;
};

#endif // !SWORD_H