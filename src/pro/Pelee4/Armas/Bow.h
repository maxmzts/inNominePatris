#ifndef BOW_H
#define BOW_H

#include <SFML/Graphics.hpp>

#include "Weapon.h"
#include "Arrow.h"
#include "AbilityType.h"
#include "Character.h"

class Bow : public Weapon {
public:
    Bow();
    void attack(sf::Vector2f position, sf::Vector2f direction std::vector<Enemy>& enemies) override;
    void useAbility(Character& character, std::vector<Enemy>& enemies) override;
    void useAbility(Character& character) override {}
    void useAbility(Character& character, sf::RenderWindow& window) override {}
    void update(float deltaTime,std::vector<Enemy>& enemies);
    void draw(sf::RenderWindow& window);

    AbilityType getAbilityType() const override { return AbilityType::Shot; };

    void increaseArrowSpeed(float speed);
    void increaseAbilityArrowCount(int count);
private:
    float arrowSpeed;
    float abilityRange;
    float abilitySpreadAngle;
    int abilityArrowCount;
    std::vector<Arrow> arrows;
};

#endif // !BOW_H