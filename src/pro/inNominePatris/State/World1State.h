#ifndef WORLD1STATE_H
#define WORLD1STATE_H

#include "WorldState.h"
#include "RoomManager.h"
#include <DungeonRoom.h>
#include <EnemyBat.h>
#include <EnemyNecromancer.h>

class World1State : public WorldState {
public:
    World1State(){
        RoomManager* roomManager = RoomManager::getInstance();
        std::shared_ptr<RoomState> room = nullptr;

        // Sala inicial, sin enemigos
        room = std::make_shared<DungeonRoom>("room1", std::vector<std::shared_ptr<Enemy>>{});
        roomManager->registerState("room1", room);

        // REGISTRAMOS LAS SALAS EN EL ROOM MANAGER
        for (int i = 2; i < 5; i++) {
            std::string roomId = "room" + std::to_string(i);
            std::vector<std::shared_ptr<Enemy>> enemies;
            // cargar enemigos (de momento son siempre los mismos en todas las salas)
            for (size_t i = 0; i < 3; i++)
            {
                enemies.push_back(std::make_shared<EnemyBat>(sf::Vector2f(100.f*i / 16,100.f*i / 16), 2));
            }
            //enemies.push_back(std::make_shared<EnemyNecromancer>(sf::Vector2f(400.f / 16,400.f / 16), 10));
            room = std::make_shared<DungeonRoom>(roomId, enemies);
            roomManager->registerState(roomId, room);
        }

        // Sala del boss
        room = std::make_shared<DungeonRoom>("room5", std::vector<std::shared_ptr<Enemy>>{});
        roomManager->registerState("room5", room);
    }

    void initialize() override {
        RoomManager* roomManager = RoomManager::getInstance();
        roomManager->changeState("room1");
    }

    void update() override {
        // Lógica específica del mundo
        // actualizar salas
        RoomManager* roomManager = RoomManager::getInstance();
        roomManager->update();
    }

    void render(GameEngine& engine) override {
        //renderizar algo si es necesario
    }
};

#endif // WORLD1STATE_H