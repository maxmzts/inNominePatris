#ifndef WORLD3STATE_H
#define WORLD3STATE_H

#include "WorldState.h"
#include "LobbyState.h"
#include "RoomManager.h"
#include <DungeonRoom.h>
#include <Boss3.h>
#include <VFXManager.h>

class World3State : public WorldState {
public:
    World3State() {
        RoomManager* roomManager = RoomManager::getInstance();
        std::shared_ptr<RoomState> room = nullptr;
    }

    void initialize() override {
        createRooms();
        RoomManager* roomManager = RoomManager::getInstance();
        roomManager->changeState("room1");
    }

    void update(TileMap& tileMap) override {
        RoomManager* roomManager = RoomManager::getInstance();
        roomManager->update(tileMap);
    }

    void render(GameEngine& engine) override {
        // Renderizar si es necesario
    }

    void createRooms() override {
        room1();
        room2();
    }

    void room1() {
        RoomManager* roomManager = RoomManager::getInstance();

        // Sala inicial, sin enemigos
        auto room = std::make_shared<DungeonRoom>(
            "room1",
            std::vector<std::shared_ptr<Enemy>>{},
            "./src/resources/music/who-killed-them.ogg",
            "./src/resources/music/hunt-the-hunter.ogg",
            [](TileMap&){}
        );

        room->setItemPositions({sf::Vector2f(35.f * 16.f, 111.f * 16.f), sf::Vector2f(57.f * 16.f, 111.f * 16.f)});

        roomManager->registerState("room1", room);
    }

    void room2() {
        RoomManager* roomManager = RoomManager::getInstance();

        std::vector<std::shared_ptr<Enemy>> enemies;
        enemies.push_back(std::make_shared<Boss3>(sf::Vector2f(44.f, 42.f), 0));

        auto room = std::make_shared<DungeonRoom>(
            "room2",
            enemies,
            "./src/resources/music/memories.ogg",
            "./src/resources/music/vengeance-part-iii.ogg",
            [](TileMap& tileMap) {
                // Efecto visual o cambios en el mapa al entrar en la sala del boss
                VFXManager::getInstance().addEffect(
                    "./src/resources/vfx/Dimensional_Portal-sheet.png",
                    {44.f*16, 25.f*16},
                    {32, 32},
                    6,
                    8,
                    false,
                    true
                );
                for (int x = 0; x < 1 ; ++x) {
                    for (int y = 0; y < 2; ++y) {
                        tileMap.setLocalTile("interaction", 44+x, 25+y, 1492);
                    }
                }
            }
        );
        
        RoomManager::getInstance()->registerState("room2", room);
    }
};

#endif // WORLD3STATE_H