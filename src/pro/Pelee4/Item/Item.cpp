#include "Item_h"

void DashBoost::applyEffect(Character& character) {
    character.increaseDashSpeed(50.0f);
}

void SpeedBoost::applyEffect(Character& character) {
    character.increaseSpeed(10.0f);
}