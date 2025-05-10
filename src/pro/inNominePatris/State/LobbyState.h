#ifndef LOBBYSTATE_H
#define LOBBYSTATE_H

#include "WorldState.h"
#include "InGame.h"

class LobbyState : public WorldState {
public:
    void update(InGame& inGame, GameEngine& engine, Character& player, TileMap& tileMap) override {
        // Lógica específica del lobby
        // Transiciones al mundo 1
        // if (player.getPosition().x > 500) { // Ejemplo: si el jugador cruza un límite
        //     inGame.changeWorldState("world_1");
        // }
    }

    void render(GameEngine& engine, Character& player, TileMap& tileMap) override {
        tileMap.draw(engine); // Renderiza el mapa del lobby
        player.draw(engine);  // Renderiza al jugador
    }
};

#endif // LOBBYSTATE_H