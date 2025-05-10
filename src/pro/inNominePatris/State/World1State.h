#ifndef WORLD1STATE_H
#define WORLD1STATE_H

#include "WorldState.h"
#include "InGame.h"

class World1State : public WorldState {
public:
    void update(InGame& inGame, GameEngine& engine, Character& player, TileMap& tileMap) override {
        // Lógica específica del mundo 1
        if (player.getHealth() <= 0) { // Si el jugador muere
            inGame.changeWorldState("lobby"); // Regresa al lobby
        }
    }

    void render(GameEngine& engine, Character& player, TileMap& tileMap) override {
        tileMap.draw(engine); // Renderiza el mapa del mundo 1
        player.draw(engine);  // Renderiza al jugador
    }
};

#endif // WORLD1STATE_H