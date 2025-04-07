#ifndef BOW_H
#define BOW_H

#include <SFML/Graphics.hpp>

#include "Weapon.h"
#include "Arrow.h"
#include "AbilityType.h"
#include "GameEngine.h"
#include "SpriteFacade.h"
#include "Character.h"
#include "Arrow.h"

class Bow : public Weapon {
public:
    Bow(GameEngine* gameEngine);
    void attack(Character& character, std::vector<Enemy>& enemies) override;
    void useAbility(Character& character, std::vector<Enemy>& enemies) override;
    void useAbility(Character& character) override {}
    void useAbility(Character& character, sf::RenderWindow& window) override {}
    void update(float deltaTime,std::vector<Enemy>& enemies);
    void draw(GameEngine& engine, const Character* character) override;
    void setPosition(float x, float y) override { spriteFacade.setPosition(x, y); };
    sf::Vector2f getPosition() const override { return spriteFacade.getPosition(); };
    AbilityType getAbilityType() const override { return AbilityType::Shot; };

    void increaseArrowSpeed(float speed);
    void increaseAbilityArrowCount(int count);
private:
    SpriteFacade spriteFacade;
    float arrowSpeed;
    float abilityRange;
    float abilitySpreadAngle;
    int abilityArrowCount;
    std::vector<Arrow> arrows;
};

#endif // !BOW_H