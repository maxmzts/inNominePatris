#ifndef MELEEWEAPON_H
#define MELEEWEAPON_H

#include "Weapon.h"

class MeleeWeapon : public Weapon {
    public:
        MeleeWeapon(float range) : attackRange(range) {}
    private:
        float attackRange;
};

#endif // !MELEEWEAPON_H