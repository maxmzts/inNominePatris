#ifndef INGAME_H
#define INGAME_H

#include "State.h"
#include "GameEngine.h"
#include "TileMap.h"
#include "../character/Character.h"
#include "../Weapon/Sword.h"
#include "../Weapon/Lance.h"
#include "../Weapon/Bow.h"
#include "../interface/HUD.h"
#include <SFML/Graphics.hpp>
#include <vector>

class InGame : public State {
private:
    static InGame* instance;
    GameEngine& engine; // Puntero al GameEngine

    TileMap tileMap;
    Character player;
    std::vector<Weapon*> weaponsOnGround;
    sf::Clock clock;

    InGame(GameEngine& engine); // Constructor privado

    HUD hud; // Instancia de la interfaz de usuario

public:
    static InGame* getInstance(GameEngine& engine); // Método para obtener la instancia
    ~InGame();  

    void update(Game& game) override;
    void render(Game& game, sf::RenderWindow& window) override;
};

#endif // INGAME_H