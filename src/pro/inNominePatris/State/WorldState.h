#ifndef WORLDSTATE_H
#define WORLDSTATE_H

#include "GameEngine.h"
#include <RoomState.h>
#include <RoomManager.h>

class InGame; // Declaración adelantada

class WorldState {
public:
    virtual ~WorldState() = default;
    virtual void initialize() = 0;

    virtual void update(TileMap& tileMap) = 0;
    virtual void render(GameEngine& engine) = 0;
};

#endif // WORLDSTATE_H