#ifndef SWORD_H
#define SWORD_H

#include "Weapon.h"
#include "SpriteFacade.h"
#include "Hitbox.h"
#include "AnimatedSprite.h"

class Sword : public Weapon {
public:
    Sword(GameEngine* engine);
    void attack(sf::Vector2f position, sf::Vector2f direction) override;
    // REIMPLEMENTAR
    bool useAbility() override;
    void useAbility(sf::Vector2f characterPosition, sf::Vector2f mousePosition) override {}
    void increaseDashSpeed(float speed);
    void decreaseDashCooldown(float cooldown);
    void decreaseAttackCooldown(float cooldown);
    void increaseAttackDamage(float damage); 
    void createHitbox(sf::Vector2f position, sf::Vector2f direction);
    // void draw(GameEngine& engine, sf::Vector2f position, sf::Vector2f direction) override;
    void render() override;
    void update(float deltaTime);
    void renderOnPlayer(sf::Vector2f position, sf::Vector2f direction) override;
    void setPosition(float x, float y) override { spriteFacade.setPosition(x, y); };
    sf::Vector2f getPosition() const override { return spriteFacade.getPosition(); };
    AbilityType getAbilityType() const override { return AbilityType::Dash; };
    float getAbilityCooldown() { return abilityCooldown; }
    float getLastAbilityTime() { return lastAbilityTime; }
    float getDashSpeed() { return dashSpeed; }
    float getDashDuration() { return 0.2f; }
    Hitbox getAttackHitbox() const override;
    float getAttackDamage() const override { return attackDamage; }

private:
    SpriteFacade spriteFacade; // Usar el Façade en lugar de sf::Sprite
    float dashSpeed;
    float abilityCooldown;
    float lastAbilityTime;
    float attackDamage = 25.0f;

    Hitbox attackHitbox;
    float attackCooldown;
    float attackTimer;

    SpriteFacade slashSpriteFacade;
    AnimatedSprite slashAnimation; // Animación de ataque
    bool isAnimating = false; // Indica si la animación está en curso
};

#endif // !SWORD_H