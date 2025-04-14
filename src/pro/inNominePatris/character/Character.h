#ifndef CHARACTER_H
#define CHARACTER_H

#include <SFML/Graphics.hpp>
#include "TileMap.h" // clase TileMap para detección de colisiones
#include "GameEngine.h" // clase GameEngine para la ventana y eventos
#include "Weapon.h" // clase Weapon para armas
#include "Hurtbox.h"
#include <vector>
#include "AbilityType.h"
#include "InteractionManager.h"
#include "AnimatedSprite.h"


class Character {
public:
    Character(const std::string& textureFile);

    void handleInput(const sf::Event& event);
    void update(const TileMap& tilemap, float deltaTime); // update recibe el mapa para verificar colisiones
    void draw(GameEngine& engine);
    
    void equipWeapon();
    sf::Vector2f getDirection() const;
    // cambiar
    // void attack(std::vector<Enemy>& enemies);
    void useAbility(sf::RenderWindow& window);
    void startDash(float speed, float duration);
    sf::FloatRect getBounds() const;
    void addWeapon(Weapon* weapon);
    void switchWeapon();
    void setWeapon(int index);
    bool hasWeapon() { if(equippedWeapon == nullptr) return false; else return true; }
    int getWeaponCount() const;
    Weapon* getEquippedWeapon() const;
    void addWeaponWithPosition(Weapon* weapon, sf::Vector2f originalPosition);
    Weapon* removeFirstWeapon(sf::Vector2f& outOriginalPosition);

    void increaseMovementSpeed(float amount);
    void increaseMaxHealth(int amount);
    
    void setPosition(float x, float y);
    sf::Vector2f getPosition() const {
        return sprite.getPosition();
    }
    void setDirection(float x, float y);
    // para spawn del jugador
    void spawnAt(const TileMap& tilemap, float x, float y);
    void interact(TileMap& tilemap);

    // para la vida del jugador
    void setHealth(int health);
    int getHealth() const;
    int getMaxHealth() const;
    void enableHealthRegeneration();
    void updateHealthRegeneration(float deltaTime);

    bool getIsInvencible() const { return isInvencible; }
    void takeDamage(int damage);
    void heal(int amount);
    void hurt(int amount);
    Hurtbox* getHurtbox() { return hurtbox; }

    // para interaccion
    void InteractionCage(TileMap& tilemap, int centerX, int centerY);
    void InteractionOpenDoor();

    // para el karma
    int getKarma() const;
    void addKarma(int amount);

    void enableTemporalyShield(float duration);

    void increaseDodgeChance(float amount);
    bool tryDodge() const;

private:
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f velocity;
    
    float speed;
    float acceleration;
    float deceleration;
    sf::Vector2f direction;

    int maxHealth;
    int currentHealth;

    bool healthRegenerationEnabled = false;
    float healthRegenTimer = 0.f;
    float healthRegenInterval = 30.f; // Intervalo de regeneración en segundos

    bool movingRight = false;
    bool movingLeft = false;
    bool movingUp = false;
    bool movingDown = false;

    bool isInvencible;
    float invencibilityTimer = 0.f;
    float invencibilityDuration = 1.f; // Duración de la invencibilidad en segundos
    AnimatedSprite* shieldAnimation; // Animación del escudo
    SpriteFacade shieldsprite;       // Textura para el escudo
    bool isShieldActive = false;     // Indica si el escudo está activo

    //Para esquivar la mejora
    float dodgeChance = 0.0f; // Probabilidad de esquivar ataques

    //Armas
    Weapon* equippedWeapon;
    bool isDashing;
    sf::Clock dashTimer;
    float dashSpeed;
    float dashDuration;
    std::vector<Weapon*> weapons; // Máximo 2 armas
    int equippedIndex = 0; // Índice del arma equipada
    std::vector<sf::Vector2f> weaponOriginalPositions; // Posiciones de las armas en el suelo
    Hurtbox* hurtbox;

    void updateInvencibility(float deltaTime);

    // Karma
    int karmaPoints = 2000; // Karma del jugador
};

#endif // CHARACTER_H
