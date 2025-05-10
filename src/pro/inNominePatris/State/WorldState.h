#ifndef WORLDSTATE_H
#define WORLDSTATE_H

#include "GameEngine.h"
#include "TileMap.h"
#include "Character.h"

class InGame; // Declaración adelantada

class WorldState {
public:
    virtual ~WorldState() = default;

    virtual void update(InGame& inGame, GameEngine& engine, Character& player, TileMap& tileMap) = 0;
    virtual void render(GameEngine& engine, Character& player, TileMap& tileMap) = 0;
};

#endif // WORLDSTATE_H