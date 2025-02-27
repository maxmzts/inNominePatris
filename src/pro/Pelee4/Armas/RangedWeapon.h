#ifndef RANGEWEAPON_H
#define RANGEWEAPON_H

#include "Weapon.h"

class RangedWeapon : public Weapon {
    protected:
        float projectilSpeed;
    public:
        RangedWeapon(float speed) : projectilSpeed(speed) {}
};

#endif // !RANGEDWEAPON_H