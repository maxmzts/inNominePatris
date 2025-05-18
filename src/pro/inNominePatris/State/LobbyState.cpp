#include "LobbyState.h"

bool LobbyState::world1Completed = false;
bool LobbyState::world2Completed = false;

void LobbyState::update(TileMap& tileMap) {
    // Desbloquear mundos si estos han sido completados
    if(world1Completed && !door2Opened) {
        openDoor2(tileMap);
        door2Opened = true;
        // DEBUG
        std::cout << "------ PUERTA MUNDO 2 ABIERTA ----------\n";
    }
    if(world2Completed && !door3Opened) {
        openDoor3(tileMap);
        door3Opened = true;
        std::cout << "------ PUERTA MUNDO 3 ABIERTA ----------\n";
    }
}

void LobbyState::render(GameEngine& engine) {
    // renderizar algo si es necesario
}

void LobbyState::openDoor2(TileMap& tileMap){
    for (int x = 28; x <= 31; ++x) {
        tileMap.removeCollisionByCoord(x, 7);
        tileMap.setLocalTile("deco", x, 7, -1);
    }
}

void LobbyState::openDoor3(TileMap& tileMap){
    for (int x = 34; x <= 37; ++x) {
        tileMap.removeCollisionByCoord(x, 7);
        tileMap.setLocalTile("deco", x, 7, -1);
    }
}