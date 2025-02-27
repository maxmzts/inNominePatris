#ifndef SWORD_H
#define SWORD_H

#include "MeleeWeapon.h"

class Sword : public MeleeWeapon {
    public:
        Sword();
        void attack(Character& character, std::vector<Enemy>& enemies) override;
        void useAbility(Character& character) override;
    private:
        float attackRange;
};

#endif // !SWORD_H