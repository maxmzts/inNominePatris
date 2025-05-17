#ifndef WORLD2S3ATE_H
#define WORLD2S3ATE_H

#include "WorldState.h"
#include "RoomManager.h"
#include <DungeonRoom.h>
#include <EnemyBat.h>
#include <EnemyNecromancer.h>

class World3State : public WorldState {
public:
    World3State(){
    }

    // void initialize() override {
    //     createRooms();
    //     RoomManager* roomManager = RoomManager::getInstance();
    //     roomManager->changeState("room1"); 
    // }

    void update(TileMap& tileMap) override {
        // Lógica específica del mundo
        // actualizar salas
        RoomManager* roomManager = RoomManager::getInstance();
        roomManager->update(tileMap);
    }

    void render(GameEngine& engine) override {
        //renderizar algo si es necesario
    }


};

#endif // WORLD3STATE_H