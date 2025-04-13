#ifndef BOW_H
#define BOW_H

#include <SFML/Graphics.hpp>

#include "Weapon.h"
#include "Arrow.h"
#include "AbilityType.h"
#include "GameEngine.h"
#include "SpriteFacade.h"
#include "Arrow.h"
#include "Hitbox.h"
#include "TileMap.h"

class Bow : public Weapon {
public:
    Bow(GameEngine* gameEngine);
    void attack(sf::Vector2f position, sf::Vector2f direction) override;
    // REIMPLEMENTAR
    bool useAbility() {}
    void useAbility(sf::Vector2f position, sf::Vector2f direction) override;
    void update(float deltaTime, const TileMap& tileMap);
    void update(float deltaTime) override {}
    void createHitbox(sf::Vector2f position, sf::Vector2f direction);
    // void draw(GameEngine& engine, sf::Vector2f position, sf::Vector2f direction) override;
    void render() override;
    void renderOnPlayer(sf::Vector2f position, sf::Vector2f direction) override;
    void setPosition(float x, float y) override { spriteFacade.setPosition(x, y); };
    sf::Vector2f getPosition() const override { return spriteFacade.getPosition(); };
    AbilityType getAbilityType() const override { return AbilityType::Shot; };
    std::shared_ptr<Hitbox> getAttackHitbox() const override{}
    void increaseArrowSpeed(float speed);
    void increaseAbilityArrowCount(int count);
    std::vector<Arrow>& getArrows() { return arrows; }
private:
    SpriteFacade spriteFacade;
    float arrowSpeed;
    float abilityRange;
    float abilitySpreadAngle;
    int abilityArrowCount;
    std::vector<Arrow> arrows;
    float attackCooldown = 0.75f; // Cooldown para el ataque (en segundos)
    float attackTimer = 0.f;     // Temporizador para el ataque
    float abilityCooldown = 4.0f; // Cooldown para la habilidad (en segundos)
    float abilityTimer = 0.f;     // Temporizador para la habilidad
};

#endif // !BOW_H