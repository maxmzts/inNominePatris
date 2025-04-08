#ifndef INGAME_H
#define INGAME_H

#include "State.h"
#include "GameEngine.h"
#include "TileMap.h"
#include "../character/Character.h"
#include "../Weapon/Sword.h"
#include "../Weapon/Lance.h"
#include "../Weapon/Bow.h"
#include <SFML/Graphics.hpp>
#include <vector>

class InGame : public State {
private:
    static InGame* instance;

    GameEngine engine;
    TileMap tileMap;
    Character player;
    std::vector<Weapon*> weaponsOnGround;

    InGame();

public:
    static InGame* getInstance();
    ~InGame();  

    void update(Game& game) override;
    void render(Game& game, sf::RenderWindow& window) override;
};

#endif // INGAME_H