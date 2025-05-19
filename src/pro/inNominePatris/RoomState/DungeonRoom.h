#ifndef DUNGEON_ROOM_H
#define DUNGEON_ROOM_H

#include "Item.h"
#include <string>
#include "RoomState.h"
#include <vector>
#include <memory>

class DungeonRoom : public RoomState {
private:
    std::string roomId;
    std::function<void(TileMap&)> openDoors;
    bool hasItems = false;
    bool doorsOpened = false;
    bool enemiesSpawned = false;
    std::vector<sf::Vector2f> itemPositions;
    std::vector<Item*> items; // Cambiado a punteros crudos
    std::string musicFilePath;
    std::string fightMusicFilePath;
    std::vector<std::shared_ptr<Enemy>> enemies; // Guardar la lista original de enemigos
    std::shared_ptr<Enemy> boss;

public:
    DungeonRoom(const std::string& id,
        std::vector<std::shared_ptr<Enemy>> enemies_list,
        const std::string& musicFilePath,
        const std::string& fightMusicFilePath = "./resources/music/hunt-the-hunter.ogg",
        std::function<void(TileMap&)> openDoors = {});
    virtual ~DungeonRoom();

    void setItemPositions(const std::vector<sf::Vector2f>& positions);
    void generateItems(Character* player);
    void enter() override;
    void update(TileMap& tileMap) override;
    void exit() override;
    void setBoss(std::shared_ptr<Enemy> boss);

    std::shared_ptr<Enemy> getBoss() const;
    const std::vector<Item*>& getItems() const;
};

#endif // DUNGEON_ROOM_H