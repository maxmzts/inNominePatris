#ifndef CHARACTER_H
#define CHARACTER_H

#include <SFML/Graphics.hpp>
#include "TileMap.h" // clase TileMap para detección de colisiones
#include "GameEngine.h" // clase GameEngine para la ventana y eventos
#include "Weapon.h" // clase Weapon para armas
#include "Enemy.h" // clase Enemy para detectar colisiones con enemigos
#include <vector>
#include "AbilityType.h"

class Character {
public:
    Character(const std::string& textureFile);

    void handleInput(const sf::Event& event);
    void update(const TileMap& tilemap, float deltaTime); // update recibe el mapa para verificar colisiones
    void draw(GameEngine& engine);
    
    void equipWeapon();
    sf::Vector2f getDirection() const;
    void attack(std::vector<Enemy>& enemies);
    void useAbility(sf::RenderWindow& window, std::vector<Enemy>& enemies);
    void startDash(float speed, float duration);
    sf::FloatRect getBounds() const;
    void addWeapon(Weapon* weapon);
    void switchWeapon();
    void setWeapon(int index);
    int getWeaponCount() const;
    Weapon* getEquippedWeapon() const;
    void addWeaponWithPosition(Weapon* weapon, sf::Vector2f originalPosition);
    Weapon* removeFirstWeapon(sf::Vector2f& outOriginalPosition);
    
    void setPosition(float x, float y);
    sf::Vector2f getPosition() const {
        return sprite.getPosition();
    }
    void setDirection(float x, float y);
    // para spawn del jugador
    void spawnAt(const TileMap& tilemap, float x, float y);

    

private:
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f velocity;
    
    float speed;
    float acceleration;
    float deceleration;
    sf::Vector2f direction;

    bool movingRight = false;
    bool movingLeft = false;
    bool movingUp = false;
    bool movingDown = false;

    //Armas
    Weapon* equippedWeapon;
    bool isDashing;
    sf::Clock dashTimer;
    float dashSpeed;
    float dashDuration;
    std::vector<Weapon*> weapons; // Máximo 2 armas
    int equippedIndex = 0; // Índice del arma equipada
    std::vector<sf::Vector2f> weaponOriginalPositions; // Posiciones de las armas en el suelo

    
};

#endif // CHARACTER_H
