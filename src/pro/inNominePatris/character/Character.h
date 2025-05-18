#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <map>
#include "TileMap.h" // Asumiendo que este header existe
#include "GameEngine.h" // Asumiendo que este header existe
#include "SpriteFacade.h" // Asumiendo que este header existe
#include "AnimatedSprite.h" // Asumiendo que este header existe
#include "InteractionManager.h"
#include "Weapon.h" // Asumiendo que este header existe

class Hurtbox; // Forward declaration

class Character {
public:
    // Enum para los estados de animación
    enum class AnimationState {
        IDLE_DOWN,
        IDLE_UP,
        IDLE_LEFT,
        IDLE_RIGHT,
        WALK_DOWN,
        WALK_UP,
        WALK_LEFT,
        WALK_RIGHT
    };

    Character();
    virtual ~Character();

    void handleInput(const sf::Event& event);
    void setMousePosition(const sf::Vector2f& position);
    void updateAimDirection();
    sf::Vector2f getAimDirection() const;
    
    virtual void update(const TileMap& tilemap, float deltaTime);
    void draw(GameEngine& engine);
    void spawnAt(const TileMap& tilemap, float x, float y);

    // Weapon related methods
    void equipWeapon();
    void startDash(float speed, float duration);
    void addWeapon(Weapon* weapon);
    void switchWeapon();
    void setWeapon(int index);
    int getWeaponCount() const;
    Weapon* getEquippedWeapon() const;
    std::vector<Weapon*> getEquippedWeapons() const { return weapons; }
    void setPosition(float x, float y);
    sf::Vector2f getDirection() const;
    void setDirection(float x, float y);
    void addWeaponWithPosition(Weapon* weapon, sf::Vector2f originalPosition);
    Weapon* removeFirstWeapon(sf::Vector2f& outOriginalPosition);
    int getEquippedIndex() const { return equippedIndex; }
    Weapon* getWeaponAtIndex(int index) const {
        if (index >= 0 && index < weapons.size()) {
            return weapons[index];
        }
        return nullptr;
    }
 

    // Health related methods
    void setHealth(int health);
    int getHealth() const;
    int getMaxHealth() const;
    void takeDamage(int damage);
    void heal(int amount);
    void hurt(int amount);
    Hurtbox* getHurtbox() { return hurtbox; }
    void drawHearts(GameEngine& engine);

    // Interaction methods
    void InteractionCage(TileMap& tilemap, int centerX, int centerY);
    void InteractionOpenDoor();
    void interact(TileMap& tilemap);
    bool getIsInvencible() const { return isInvencible; }
 
    
    void updateInvencibility(float deltaTime);
    int getKarma() const;
    void addKarma(int amount);
    void resetKarma();
    void enableCoinBonus();
    void updateHealthRegeneration(float deltaTime);
    bool tryDodge() const;
    void increaseMovementSpeed(float amount);
    void increaseDodgeChance(float amount);
    void increaseMaxHealth(int amount);
    void enableHealthRegeneration();
    void enableTemporalyShield(float duration);
    
    sf::FloatRect getBounds() const;
    
    sf::Vector2f getPosition() const { return sprite.getPosition(); }


    static Character* getInstance();
    void static setInstance(Character* instance);

    void reset();

protected:
    // Animation methods
    void setupAnimations();
    void updateAnimation();
    void changeAnimationState(AnimationState newState);

    SpriteFacade sprite;
    sf::Texture texture;
    sf::Vector2f velocity;
    sf::Vector2f direction;
    sf::Vector2f mousePosition;
    sf::Vector2f aimDirection;
    sf::CircleShape collisionCircle;
    
    float speed;
    float acceleration;
    float deceleration;
    
    bool movingLeft = false;
    bool movingRight = false;
    bool movingUp = false;
    bool movingDown = false;
    
    bool isDashing;
    float dashSpeed;
    float dashDuration;
    sf::Clock dashTimer;
    
    std::vector<Weapon*> weapons;
    std::vector<sf::Vector2f> weaponOriginalPositions;
    Weapon* equippedWeapon;
    int equippedIndex;
    
    int maxHealth;
    float currentHealth;
    
    bool isInvencible;
    float invencibilityTimer = 0;
    float invencibilityDuration = 1.0f;
    
    Hurtbox* hurtbox; // Hurtbox para detección de colisión con enemigos
    
    static Character* instance; // Instancia única

    // Shield related
    bool isShieldActive = false;
    SpriteFacade shieldsprite;
    AnimatedSprite* shieldAnimation;
    
    // Health regeneration
    bool healthRegenerationEnabled = false;
    float healthRegenTimer = 0.0f;
    float healthRegenInterval = 5.0f; // Regenerar cada 5 segundos
    
    // Dodge chance
    float dodgeChance = 0.0f;
    
    // Karma system
    bool CoinBonusEnabled = false;
    int karmaPoints = 2000;
    
    // Animation system
    AnimatedSprite* animations;
    AnimationState currentState;
    bool wasMoving;
    std::map<AnimationState, std::string> animationTextures;

    void initCollisionCircle();
};