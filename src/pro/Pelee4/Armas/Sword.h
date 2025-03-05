#ifndef SWORD_H
#define SWORD_H

#include <SFML/Graphics.hpp>

#include "MeleeWeapon.h"

class Sword : public MeleeWeapon {
    public:
        Sword();
        void attack(Character& character, std::vector<Enemy>& enemies) override;
        void useAbility(Character& character) override;
        void increaseDashSpeed(float speed);
    private:
        float attackRange;
        float dashSpeed;
};

#endif // !SWORD_H