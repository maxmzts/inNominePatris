#ifndef DUNGEON_ROOM_H
#define DUNGEON_ROOM_H

#include <string>
#include "RoomState.h"

// Ejemplo de una sala concreta
class DungeonRoom : public RoomState {
private:
    std::string roomId;
    
public:
    DungeonRoom(const std::string& id, std::vector<std::shared_ptr<Enemy>> enemies_list);
    virtual ~DungeonRoom();
    
    void enter() override;
    void update() override;
    void exit() override;
};

#endif // DUNGEON_ROOM_H