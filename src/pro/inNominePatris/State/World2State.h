#ifndef WORLD2STATE_H
#define WORLD2STATE_H

#include "WorldState.h"
#include "RoomManager.h"
#include <DungeonRoom.h>
#include <EnemyBat.h>
#include <EnemyNecromancer.h>

class World2State : public WorldState {
public:
    World2State(){
        RoomManager* roomManager = RoomManager::getInstance();
        std::shared_ptr<RoomState> room = nullptr;

        createRooms();
    }

    void initialize() override {
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

    void createRooms(){
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
        room = std::make_shared<DungeonRoom>("room1", std::vector<std::shared_ptr<Enemy>>{}, [](TileMap&) {/*No hace nada*/});
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
            [](TileMap& tileMap) {
                // Puerta horizontal: de (116, 45) a (121, 45)
                for (int x = 116; x <= 121; ++x) {
                    tileMap.removeCollisionByCoord(x, 45);
                }
        
                // Puerta vertical: de (143, 28) a (143, 23)
                for (int y = 28; y >= 23; --y) {
                    tileMap.removeCollisionByCoord(143, y);
                }
            }
        );
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
            [](TileMap& tileMap) {
                // Puerta horizontal: de (116, 45) a (121, 45)
                for (int x = 116; x <= 121; ++x) {
                    tileMap.removeCollisionByCoord(x, 45);
                }
        
                // Puerta vertical: de (143, 28) a (143, 23)
                for (int y = 28; y >= 23; --y) {
                    tileMap.removeCollisionByCoord(143, y);
                }
            }
        );
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
            [](TileMap& tileMap) {
                // Puerta horizontal: de (116, 45) a (121, 45)
                for (int x = 116; x <= 121; ++x) {
                    tileMap.removeCollisionByCoord(x, 45);
                }
        
                // Puerta vertical: de (143, 28) a (143, 23)
                for (int y = 28; y >= 23; --y) {
                    tileMap.removeCollisionByCoord(143, y);
                }
            }
        );
        RoomManager* roomManager = RoomManager::getInstance();
        roomManager->registerState("room4", room);        
    }

    void room5(){
        std::vector<std::shared_ptr<Enemy>> enemies;
        
        // Boss

        // crear la sala
        auto room = std::make_shared<DungeonRoom>(
            "room5",
            enemies,
            [](TileMap& tileMap) {
                // Puerta horizontal: de (116, 45) a (121, 45)
                for (int x = 116; x <= 121; ++x) {
                    tileMap.removeCollisionByCoord(x, 45);
                }
        
                // Puerta vertical: de (143, 28) a (143, 23)
                for (int y = 28; y >= 23; --y) {
                    tileMap.removeCollisionByCoord(143, y);
                }
            }
        );
        RoomManager* roomManager = RoomManager::getInstance();
        roomManager->registerState("room5", room);        
    }
};

#endif // WORLD2STATE_H