#ifndef INGAME_H
#define INGAME_H

#include "KarmaSystem.h"
#include "State.h"
#include "GameEngine.h"
#include "TileMap.h"
#include "Character.h"
#include "Sword.h"
#include "Lance.h"
#include "Bow.h"
#include "Enemy.h"
#include "Shop.h"
#include "HUD.h"
#include "WorldState.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <VFXManager.h>

class InGame : public State {
private:
    static InGame* instance;
    GameEngine& engine; // Puntero al GameEngine
    Character player;
    KarmaSystem karmaSystem; // Sistema de karma
    Shop shop;

    TileMap tileMap;
    std::vector<Weapon*> weaponsOnGround;
    sf::Clock clock;

    InGame(GameEngine& engine); // Constructor privado

    HUD hud; // Instancia de la interfaz de usuario
    
    // Método para verificar interacciones automáticas (como los teletransportes)
    void checkAutoInteractions();

    std::string proximityMessage;
    sf::Font font;

    std::unordered_map<std::string, std::unique_ptr<WorldState>> worldStates;
    WorldState* currentWorldState;

public:
    static InGame* getInstance(GameEngine& engine); // Método para obtener la instancia
    ~InGame();  

    void update(Game& game) override;
    void render(Game& game, sf::RenderWindow& window) override;

    void reset(GameEngine& engine);

    bool checkEnemyWasHit(std::shared_ptr<Enemy> enemy, Character player);
    bool checkPlayerWasHit(Character& player, std::shared_ptr<Enemy> enemy);

    void changeWorldState(const std::string& stateName);
};

#endif // INGAME_H