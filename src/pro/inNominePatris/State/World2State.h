#ifndef WORLD2STATE_H
#define WORLD2STATE_H

#include "WorldState.h"
#include "RoomManager.h"
#include <DungeonRoom.h>
#include <EnemyBat.h>
#include <EnemyNecromancer.h>
#include <Boss1.h>
#include <Boss2.h>
#include "LobbyState.h"

class World2State : public WorldState {
public:
    World2State(){
    }

    void initialize() override {
        createRooms();
        RoomManager* roomManager = RoomManager::getInstance();
        roomManager->changeState("room1"); 
    }

    void update(TileMap& tileMap) override {
        // Lógica específica del mundo
        // actualizar salas
        RoomManager* roomManager = RoomManager::getInstance();
        roomManager->update(tileMap);
    }

    void render(GameEngine& engine) override {
        //renderizar algo si es necesario
    }

    void createRooms() override{
        room1();
        room2();
        room3();
        room4();
        room5();
    }

    void room1(){
        RoomManager* roomManager = RoomManager::getInstance();
        std::shared_ptr<RoomState> room = nullptr;
        // Sala inicial, sin enemigos
        room = std::make_shared<DungeonRoom>(
            "room1", 
            std::vector<std::shared_ptr<Enemy>>{},
            "./resources/music/dungeon-crawler.ogg",
            "./resources/music/hunt-the-hunter.ogg");
        roomManager->registerState("room1", room);
    }

    void room2(){
        std::vector<std::shared_ptr<Enemy>> enemies;
        
        enemies.push_back(std::make_shared<EnemyBat>(sf::Vector2f(136.f, 17.f), 2));
        enemies.push_back(std::make_shared<EnemyBat>(sf::Vector2f(140.f, 17.f), 2));
        enemies.push_back(std::make_shared<EnemyBat>(sf::Vector2f(140.f, 23.f), 2));
        enemies.push_back(std::make_shared<EnemyNecromancer>(sf::Vector2f(138.f, 7.f), 2));
        enemies.push_back(std::make_shared<EnemyNecromancer>(sf::Vector2f(157.f, 7.f), 2));
        enemies.push_back(std::make_shared<EnemyNecromancer>(sf::Vector2f(157.f, 22.f), 2));

        // crear la sala
        auto room = std::make_shared<DungeonRoom>(
            "room2",
            enemies,
            "./resources/music/dungeon-crawler.ogg",
            "./resources/music/hunt-the-hunter.ogg",
            [](TileMap& tileMap) {
                for (int x = 126; x <= 131; ++x) {
                    tileMap.removeCollisionByCoord(x, 35);
                    tileMap.setLocalTile("deco", x, 35, -1);
                }
            }
        );

        room->setItemPositions({sf::Vector2f(145.f * 16.f, 14.f * 16.f), sf::Vector2f(145.f * 16.f, 20.f * 16.f)});

        RoomManager* roomManager = RoomManager::getInstance();
        roomManager->registerState("room2", room);        
    }

    void room3(){
        std::vector<std::shared_ptr<Enemy>> enemies;
        
        enemies.push_back(std::make_shared<EnemyBat>(sf::Vector2f(159.f, 74.f), 2));
        enemies.push_back(std::make_shared<EnemyBat>(sf::Vector2f(159.f, 107.f), 2));
        enemies.push_back(std::make_shared<EnemyBat>(sf::Vector2f(196.f, 74.f), 2));
        enemies.push_back(std::make_shared<EnemyBat>(sf::Vector2f(196.f, 107.f), 2));
        enemies.push_back(std::make_shared<EnemyNecromancer>(sf::Vector2f(196.f, 74.f), 2));
        enemies.push_back(std::make_shared<EnemyNecromancer>(sf::Vector2f(196.f, 107.f), 2));
        enemies.push_back(std::make_shared<EnemyNecromancer>(sf::Vector2f(189.f, 91.f), 2));

        // crear la sala
        auto room = std::make_shared<DungeonRoom>(
            "room3",
            enemies,
            "./resources/music/dungeon-crawler.ogg",
            "./resources/music/hunt-the-hunter.ogg",
            [](TileMap& tileMap) {
                for (int x = 150, y = 90; y >= 85; --y) {
                    tileMap.removeCollisionByCoord(x, y);
                    tileMap.setLocalTile("deco", x, y, -1);
                }
            }
        );
        
        room->setItemPositions({sf::Vector2f(170.f * 16.f, 90.f * 16.f), sf::Vector2f(179.f * 16.f, 90.f * 16.f)});

        RoomManager* roomManager = RoomManager::getInstance();
        roomManager->registerState("room3", room);        
    }

    void room4(){
        std::vector<std::shared_ptr<Enemy>> enemies;
        
        enemies.push_back(std::make_shared<EnemyBat>(sf::Vector2f(84.f, 189.f), 2));
        enemies.push_back(std::make_shared<EnemyBat>(sf::Vector2f(114.f, 189.f), 2));
        enemies.push_back(std::make_shared<EnemyBat>(sf::Vector2f(91.f, 201.f), 2));
        enemies.push_back(std::make_shared<EnemyBat>(sf::Vector2f(105.f, 201.f), 2));
        enemies.push_back(std::make_shared<EnemyNecromancer>(sf::Vector2f(84.f, 208.f), 2));
        enemies.push_back(std::make_shared<EnemyNecromancer>(sf::Vector2f(114.f, 208.f), 2));
        enemies.push_back(std::make_shared<EnemyNecromancer>(sf::Vector2f(98.f, 191.f), 2));

        // crear la sala
        auto room = std::make_shared<DungeonRoom>(
            "room4",
            enemies,
            "./resources/music/dungeon-crawler.ogg",
            "./resources/music/hunt-the-hunter.ogg",
            [](TileMap& tileMap) {
                for (int x = 95, y = 159; x <= 100; ++x) {
                    tileMap.removeCollisionByCoord(x, y);
                    tileMap.setLocalTile("deco", x, y, -1);
                }
            }
        );

        room->setItemPositions({sf::Vector2f(93.f * 16.f, 190.f * 16.f), sf::Vector2f(102.f * 16.f, 190.f * 16.f)});

        RoomManager* roomManager = RoomManager::getInstance();
        roomManager->registerState("room4", room);        
    }

    void room5(){
        std::vector<std::shared_ptr<Enemy>> enemies;
        
        // Boss
        enemies.push_back(std::make_shared<Boss2>(sf::Vector2f(155.f * 16.f, 273.f * 16.f),1000));
        // crear la sala
        auto room = std::make_shared<DungeonRoom>(
            "room5",
            enemies,
            "./resources/music/memories.ogg",
            "./resources/music/vengeance-part-ii.ogg",
            [](TileMap& tileMap) {
                for (int x = 0; x < 2 ; ++x) {
                    for (int y = 0; y < 3; ++y) {
                        tileMap.setLocalTile("interaction", 155+x, 280+y, 1492);
                    }
                }
                LobbyState::setWorld2completed();
            }
        );
        RoomManager* roomManager = RoomManager::getInstance();
        roomManager->registerState("room5", room);        
    }
};

#endif // WORLD2STATE_H