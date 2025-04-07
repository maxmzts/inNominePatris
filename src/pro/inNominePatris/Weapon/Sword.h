#ifndef SWORD_H
#define SWORD_H

#include "Weapon.h"
#include "SpriteFacade.h"

class Sword : public Weapon {
public:
    Sword(GameEngine* engine);
    void attack(Character& character, std::vector<Enemy>& enemies) override;
    void useAbility(Character& character) override;
    void increaseDashSpeed(float speed);
    void increaseAttackRange(float range);
    void draw(GameEngine& engine,const Character* character) override;
    void setPosition(float x, float y) override { spriteFacade.setPosition(x, y); };
    sf::Vector2f getPosition() const override { return spriteFacade.getPosition(); };
    AbilityType getAbilityType() const override { return AbilityType::Dash; };

private:
    SpriteFacade spriteFacade; // Usar el Façade en lugar de sf::Sprite
    float attackRange;
    float dashSpeed;
    float abilityCooldown;
    float lastAbilityTime;
};

#endif // !SWORD_H