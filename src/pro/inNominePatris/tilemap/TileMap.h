#pragma once
#include <SFML/Graphics.hpp>
#include "tinyxml2.h"
#include "Tile.h"
#include "GameEngine.h"
#include <vector>
#include <unordered_map>
#include <map>
#include <string>

class TileMap : public sf::Drawable, public sf::Transformable {
public:
    bool loadFromFile(const std::string& filename, GameEngine& engine);
    bool isColliding(const sf::FloatRect& playerBounds) const;
    void draw(GameEngine& engine) const;
    
    // Verificar posición de spawn válida
    sf::Vector2f getSpawnPosition(float x, float y) const;
    bool isPlayerInteractingWithTile(const sf::FloatRect& playerBounds, int& outTileId) const;
    void addInteractiveTile(int id, const sf::FloatRect& rect);
    
    // Nuevo método para modificar tiles en cualquier capa
    bool setTile(const std::string& layerName, int x, int y, int tileId);
    bool setLocalTile(const std::string& layerName, int x, int y, int localTileId);

    
private:
    struct TileSet {
        int firstGid;
        sf::Texture texture;
        int tileWidth, tileHeight, columns;
    };

    struct Layer {
        sf::VertexArray vertices;
        const sf::Texture* tilesetTexture;
        TileSet* tileset;           // Referencia al tileset usado
        std::vector<int> tiles;     // Almacenar los IDs de los tiles
        std::string name;           // Nombre de la capa
    };

    struct InteractiveTile {
        int id;             // ID del tile interactivo
        sf::FloatRect rect; // Rectángulo de colisión
    };

    std::vector<Layer> m_layers;
    std::unordered_map<int, TileSet> m_tilesets;
    std::vector<InteractiveTile> interactiveTiles;
    std::vector<int> m_boundsTiles;                // Almacena los tiles de colisión
    std::map<std::string, int> m_layerIndices;     // Mapeo de nombres de capa a índices

    int m_mapWidth, m_mapHeight;

    std::vector<sf::FloatRect> collisionBlocks;    // Almacena los bloques de colisión

    // Atributos privados
    sf::VertexArray m_vertices;
    sf::Texture m_tileset;

    // Funciones auxiliares
    void updateLayerVertices(Layer& layer, TileSet* tileset);
    void updateCollisionBlocks(TileSet* tileset);
    void updateSingleTile(Layer& layer, int x, int y, int oldTileId, int newTileId);



    // Método privado para dibujar (sobrescribe sf::Drawable)
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override{};
};