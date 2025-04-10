#pragma once
#include <SFML/Graphics.hpp>
#include "tinyxml2.h"
#include "Tile.h"
#include "GameEngine.h"
#include <vector>
#include <unordered_map>

class TileMap : public sf::Drawable, public sf::Transformable {
public:
    bool loadFromFile(const std::string& filename, GameEngine& engine);
    bool isColliding(const sf::FloatRect& playerBounds) const; // NUEVA FUNCIÓN
    void draw(GameEngine& engine) const;
    
    // verificar posición de spawn válida
    sf::Vector2f getSpawnPosition(float x, float y) const;
    bool isPlayerInteractingWithTile(const sf::FloatRect& playerBounds, int& outTileId) const;
    void addInteractiveTile(int id, const sf::FloatRect& rect);

    
private:
    struct TileSet {
        int firstGid;
        sf::Texture texture;
        int tileWidth, tileHeight, columns;
    };

    struct Layer {
        sf::VertexArray vertices;
        const sf::Texture* tilesetTexture;
    };

    struct InteractiveTile {
        int id;             // ID del tile interactivo
        sf::FloatRect rect; // Rectángulo de colisión
    };

    std::vector<Layer> m_layers;
    std::unordered_map<int, TileSet> m_tilesets;
    std::vector<InteractiveTile> interactiveTiles;

    int m_mapWidth, m_mapHeight;

    std::vector<sf::FloatRect> collisionBlocks; // ALMACENA LOS BLOQUES DE COLISIÓN

    // Atributos privados
    sf::VertexArray m_vertices;
    sf::Texture m_tileset;

    // Método privado para dibujar (sobrescribe sf::Drawable)
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override{};
};