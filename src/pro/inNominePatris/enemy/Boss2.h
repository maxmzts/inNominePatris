#ifndef BOSS2_H
#define BOSS2_H

#include "Enemy.h"
#include "Character.h"
#include "TileMap.h"
#include "Hitbox.h"
#include "Hurtbox.h"

// Enum para los estados del Boss2
enum class Boss2State {
    IDLE = 0,
    MOVING = 1,
    ATTACKING_MELEE = 2,
    ATTACKING_RANGED = 3,
    HURT = 4,
    DYING = 5,
    DEAD = 6
};

class Boss2 : public Enemy {
public:
    Boss2(const sf::Vector2f& startPosition, int DropKarmaPoints = 100);
    ~Boss2();

    // Métodos principales
    void update(float deltaTime, Character* player, const TileMap* tileMap) override;
    void render(sf::RenderWindow& window);
    void takeDamage(float damage, const sf::Vector2f& attackPosition) override;
    bool isDead() const override;
    int getKarmaPoints() const;

private:
    // Estado y timers
    Boss2State currentBoss2State;
    float idleTimer;
    float moveTimer;
    float attackCooldown;
    float attackTimer;
    float invincibilityDuration;
    float stateTimer;          // Timer para controlar la duración de los estados

    // Rangos de ataque
    float meleeRange;
    float rangedRange;

    bool secondRangedAttackPending = false;
    float secondRangedAttackTimer = 0.f;

    // Ataque
    int attackPattern;
    float projectileSpeed;
    bool attacked;             // Flag para controlar si ya atacó en este ciclo
    Hitbox* attackHitbox;      // Hitbox específica para los ataques

    // Puntos de karma al derrotar
    int KarmaPoints;

    // Métodos internos
    void loadAnimations();
    void changeAnimation(int newStateInt);
    void calculateVelocity(const sf::Vector2f& direction);
    void attack() override {};
    void attackMelee();
    void attackRanged(const sf::Vector2f& playerPos);
    void updateTimers(float deltaTime);
    void updateHitboxes();
    void changeState(int newStateInt);
};

#endif // BOSS2_H