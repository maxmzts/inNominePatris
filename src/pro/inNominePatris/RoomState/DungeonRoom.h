#ifndef DUNGEON_ROOM_H
#define DUNGEON_ROOM_H

#include "Item.h"
#include <string>
#include "RoomState.h"

class DungeonRoom : public RoomState {
private:
    std::string roomId;
    std::function<void(TileMap&)> openDoors;
    std::vector<sf::Vector2f> itemPositions;
    std::vector<std::unique_ptr<Item>> items;

public:
    DungeonRoom(const std::string& id, 
                std::vector<std::shared_ptr<Enemy>> enemies_list,
                std::function<void(TileMap&)> openDoors);
    virtual ~DungeonRoom();

    void setItemPositions(const std::vector<sf::Vector2f>& positions);
    void generateItems(Character* player);
    void enter() override;
    void update(TileMap& tileMap) override;
    void exit() override;

    const std::vector<std::unique_ptr<Item>>& getItems() const { return items; }
};

#endif // DUNGEON_ROOM_H