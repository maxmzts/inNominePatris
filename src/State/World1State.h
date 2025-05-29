#ifndef WORLD1STATE_H
#define WORLD1STATE_H

#include "WorldState.h"
#include "LobbyState.h"
#include "RoomManager.h"
#include <DungeonRoom.h>
#include <EnemyBat.h>
#include <EnemyNecromancer.h>
#include <Boss1.h>
#include <Boss2.h>
#include <Boss3.h>
#include <VFXManager.h>

class World1State : public WorldState {
public:
    World1State(){
        RoomManager* roomManager = RoomManager::getInstance();
        std::shared_ptr<RoomState> room = nullptr;
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
            // std::vector<std::shared_ptr<Enemy>>{std::make_shared<Boss1>(sf::Vector2f(20.f,10.f), 0)},
            // std::vector<std::shared_ptr<Enemy>>{std::make_shared<Boss3>(sf::Vector2f(20.f, 20.f), 2)},
            "./src/resources/music/who-killed-them.ogg",
            "./src/resources/music/hunt-the-hunter.ogg");
        roomManager->registerState("room1", room);
    }

    void room2(){
        std::vector<std::shared_ptr<Enemy>> enemies;

        enemies.push_back(std::make_shared<EnemyBat>(sf::Vector2f(103.f, 12.f), 2));
        enemies.push_back(std::make_shared<EnemyBat>(sf::Vector2f(136.f, 11.f), 2));
        enemies.push_back(std::make_shared<EnemyBat>(sf::Vector2f(120.f, 24.f), 2));
        enemies.push_back(std::make_shared<EnemyBat>(sf::Vector2f(120.f, 20.f), 2));

        // crear la sala
        auto room = std::make_shared<DungeonRoom>(
            "room2",
            enemies,
            "./src/resources/music/who-killed-them.ogg",
            "./src/resources/music/hunt-the-hunter.ogg",
            [](TileMap& tileMap) {
                // Puerta horizontal: de (116, 45) a (121, 45)
                for (int x = 116; x <= 121; ++x) {
                    tileMap.removeCollisionByCoord(x, 45);
                    tileMap.setLocalTile("deco", x, 44, -1);
                }
        
                // Puerta vertical: de (143, 28) a (143, 23)
                for (int y = 28; y >= 23; --y) {
                    tileMap.removeCollisionByCoord(143, y);
                }
                for (int y = 27; y >= 24; --y) {
                    tileMap.setLocalTile("deco", 142, y, -1);
                }
            }
        );

        room->setItemPositions({sf::Vector2f(113.f * 16.f, 25.f * 16.f), sf::Vector2f(125.f * 16.f, 25.f * 16.f)});

        RoomManager* roomManager = RoomManager::getInstance();
        roomManager->registerState("room2", room);        
    }

    void room3(){
        std::vector<std::shared_ptr<Enemy>> enemies;
        
        enemies.push_back(std::make_shared<EnemyBat>(sf::Vector2f(197.f, 43.f), 2));
        enemies.push_back(std::make_shared<EnemyBat>(sf::Vector2f(197.f, 10.f), 2));
        enemies.push_back(std::make_shared<EnemyBat>(sf::Vector2f(236.f, 10.f), 2));
        enemies.push_back(std::make_shared<EnemyBat>(sf::Vector2f(236.f, 43.f), 2));
        enemies.push_back(std::make_shared<EnemyNecromancer>(sf::Vector2f(223.f, 26.f), 10));

        auto room = std::make_shared<DungeonRoom>(
            "room3",
            enemies,
            "./src/resources/music/who-killed-them.ogg",
            "./src/resources/music/hunt-the-hunter.ogg",
            [](TileMap& tileMap) {
                for (int y = 23; y <= 28; ++y) {
                    tileMap.removeCollisionByCoord(190, y);
                }
                for (int y = 24; y <= 27; ++y) {
                    tileMap.setLocalTile("deco", 190, y, -1);
                }
                for (int x = 211; x <= 216; ++x) {
                    tileMap.removeCollisionByCoord(x, 47);
                }
                for (int x = 212; x <= 215; ++x) {
                    tileMap.setLocalTile("deco", x, 47, -1);
                }
            }
        );

        room->setItemPositions({sf::Vector2f(219.f * 16.f, 25.f * 16.f), sf::Vector2f(226.f * 16.f, 25.f * 16.f)});


        RoomManager* roomManager = RoomManager::getInstance();
        roomManager->registerState("room3", room); 
    }

    void room4(){
        std::vector<std::shared_ptr<Enemy>> enemies;
        
        enemies.push_back(std::make_shared<EnemyBat>(sf::Vector2f(214.f, 110.f), 2));
        enemies.push_back(std::make_shared<EnemyBat>(sf::Vector2f(214.f, 101.f), 2));
        enemies.push_back(std::make_shared<EnemyBat>(sf::Vector2f(214.f, 92.f), 2));
        enemies.push_back(std::make_shared<EnemyNecromancer>(sf::Vector2f(209.f, 117.f), 10));
        enemies.push_back(std::make_shared<EnemyNecromancer>(sf::Vector2f(221.f, 117.f), 10));
        
        auto room = std::make_shared<DungeonRoom>(
            "room4",
            enemies,
            "./src/resources/music/who-killed-them.ogg",
            "./src/resources/music/hunt-the-hunter.ogg",
            [](TileMap& tileMap) {
                // Puerta horizontal: de (212,80) a (214,80)
                for (int x = 212; x <= 214; ++x) {
                    tileMap.removeCollisionByCoord(x, 80);
                }
                for (int x = 213; x <= 216; ++x) {
                    tileMap.setLocalTile("deco", x, 80, -1);
                }
                // Puerta vertical: de (225,115) a (225,120)
                for (int y = 115; y <= 120; ++y) {
                    tileMap.removeCollisionByCoord(225, y);
                }
                for (int y = 116; y <= 119; ++y) {
                    tileMap.setLocalTile("deco", 225, y, -1);
                }
            }
        );
        
        room->setItemPositions({sf::Vector2f(211.f * 16.f, 100.f * 16.f), sf::Vector2f(218.f * 16.f, 100.f * 16.f)});

        RoomManager* roomManager = RoomManager::getInstance();
        roomManager->registerState("room4", room); 
    }

    void room5(){

        auto room = std::make_shared<DungeonRoom>(
            "room5",
            std::vector<std::shared_ptr<Enemy>>{std::make_shared<Boss1>(sf::Vector2f(220.f,192.f), 200)},
            "./src/resources/music/memories.ogg",
            "./src/resources/music/vengeance-part-i.ogg",
            [](TileMap& tileMap) {
                VFXManager::getInstance().addEffect(
                    "./src/resources/vfx/Dimensional_Portal-sheet.png",
                    {240.f*16,188.f*16},
                    {32, 32},
                    6,
                    8, 
                    false,
                    true 
                );
                for (int x = 0; x < 1; ++x) {
                    for( int y = 0; y < 2; ++y){
                        tileMap.setLocalTile("interaction", 240+x, 188+y, 1492);
                    }
                }
                LobbyState::setWorld1completed();
            }
        );
        RoomManager* roomManager = RoomManager::getInstance();
        roomManager->registerState("room5", room); 
    }
};

#endif // WORLD1STATE_H