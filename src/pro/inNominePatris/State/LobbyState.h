#ifndef LOBBYSTATE_H
#define LOBBYSTATE_H

#include "WorldState.h"
#include "InGame.h"

class LobbyState : public WorldState {
public:
    void initialize() override {
        // logica para cuando se carga el lobby si es necesario
    }

    void update(TileMap& tileMap) override {
        // Desbloquear mundos si estos han sido completados
        // if(mundo1-completed) {
        //    openDoor2()
        // }
        // if(mundo2-completed) {
        //    openDoor3()
        // }
    }

    void render(GameEngine& engine) override {
        // renderizar algo si es necesario
    }
};

#endif // LOBBYSTATE_H