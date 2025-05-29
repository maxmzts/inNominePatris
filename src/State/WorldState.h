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

    // "virtual opcional"
    virtual void createRooms() { /* Por defecto no hace nada */}

    // para reintentar una run
    void reload(){ clearRooms(); createRooms();};

    // para obtener las recompensas
    void clearRooms(){ RoomManager::getInstance()->clearRooms(); }
};

#endif // WORLDSTATE_H