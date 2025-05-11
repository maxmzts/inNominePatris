#ifndef LOBBYSTATE_H
#define LOBBYSTATE_H

#include "WorldState.h"
#include "InGame.h"

class LobbyState : public WorldState {
public:
    void initialize() override {
        // logica para cuando se carga el lobby si es necesario
    }

    void update() override {
        // Lógica específica del lobby
        // Transiciones al mundo 1
        // if (player.getPosition().x > 500) { // Ejemplo: si el jugador cruza un límite
        //     inGame.changeWorldState("world_1");
        // }
    }

    void render(GameEngine& engine) override {
        // renderizar algo si es necesario
    }
};

#endif // LOBBYSTATE_H