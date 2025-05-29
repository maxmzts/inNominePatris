#ifndef LOBBYSTATE_H
#define LOBBYSTATE_H

#include "WorldState.h"
#include "InGame.h"

class LobbyState : public WorldState {
private:
    static bool world1Completed;
    static bool world2Completed;
    bool door2Opened = false;
    bool door3Opened = false;
public:
    void initialize() override { 
        door2Opened = false; door3Opened = false;
    }

    void update(TileMap& tileMap) override;

    void render(GameEngine& engine) override;

    static void setWorld1completed(){ world1Completed = true; }

    static void setWorld2completed(){ world2Completed = true; }
    
    static bool isWorld1Completed() { return world1Completed; }
    
    static bool isWorld2Completed() { return world2Completed; }

    void openDoor2(TileMap& tileMap);

    void openDoor3(TileMap& tileMap);

    void spawnWeaponsOnGround(std::vector<Weapon*>& weaponsOnGround, GameEngine* engine);
};

#endif // LOBBYSTATE_H