#include "TileMap.h"
#include <iostream>
#include <sstream>


bool TileMap::loadFromFile(const std::string& filename, GameEngine& engine) {
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filename.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Error cargando el mapa XML\n";
        return false;
    }

    tinyxml2::XMLElement* mapElement = doc.FirstChildElement("map");
    if (!mapElement) return false;

    mapElement->QueryIntAttribute("width", &m_mapWidth);
    mapElement->QueryIntAttribute("height", &m_mapHeight);

    TileSet* mainTileset = nullptr;
    TileSet* decoTileset = nullptr;

    for (tinyxml2::XMLElement* tilesetElement = mapElement->FirstChildElement("tileset");
         tilesetElement; tilesetElement = tilesetElement->NextSiblingElement("tileset")) {
        
        int firstGid;
        tilesetElement->QueryIntAttribute("firstgid", &firstGid);
        
        tinyxml2::XMLElement* imageElement = tilesetElement->FirstChildElement("image");
        if (!imageElement) continue;
        
        const char* source = imageElement->Attribute("source");
        if (!source) continue;

        TileSet tileset;
        tileset.firstGid = firstGid;
        tileset.texture = engine.loadTexture(std::string("resources/") + source); // Usar el motor para cargar texturas
        tilesetElement->QueryIntAttribute("tilewidth", &tileset.tileWidth);
        tilesetElement->QueryIntAttribute("tileheight", &tileset.tileHeight);
        tileset.columns = tileset.texture.getSize().x / tileset.tileWidth;

        m_tilesets[firstGid] = tileset;

        if (!mainTileset) {
            mainTileset = &m_tilesets[firstGid];
        } else {
            decoTileset = &m_tilesets[firstGid];
        }
    }

    for (tinyxml2::XMLElement* layerElement = mapElement->FirstChildElement("layer");
         layerElement; layerElement = layerElement->NextSiblingElement("layer")) {
        
        const char* layerName = layerElement->Attribute("name");
        bool isDecoLayer = (layerName && std::string(layerName) == "deco");
        bool isBoundsLayer = (layerName && std::string(layerName) == "bounds");
        bool isInteractiveLayer = (layerName && std::string(layerName) == "interaction");

        tinyxml2::XMLElement* dataElement = layerElement->FirstChildElement("data");
        if (!dataElement) continue;

        std::vector<int> tiles;
        const char* csvData = dataElement->GetText();
        std::stringstream ss(csvData);
        std::string tileID;
        
        while (std::getline(ss, tileID, ',')) {
            tiles.push_back(std::stoi(tileID));
        }

        if (isBoundsLayer) {
            for (int y = 0; y < m_mapHeight; ++y) {
                for (int x = 0; x < m_mapWidth; ++x) {
                    int tileNumber = tiles[y * m_mapWidth + x];
                    if (tileNumber != 0) {
                        collisionBlocks.push_back(sf::FloatRect(x * mainTileset->tileWidth, y * mainTileset->tileHeight,
                                                                mainTileset->tileWidth, mainTileset->tileHeight));
                    }
                }
            }
            continue; 
        }
        
        // Procesar la capa de tiles interactivos
        if (isInteractiveLayer) {
            for (int y = 0; y < m_mapHeight; ++y) {
                for (int x = 0; x < m_mapWidth; ++x) {
                    int tileNumber = tiles[y * m_mapWidth + x];
                    if (tileNumber != 0) {
                        addInteractiveTile(tileNumber, sf::FloatRect(
                            x * mainTileset->tileWidth, 
                            y * mainTileset->tileHeight,
                            mainTileset->tileWidth, 
                            mainTileset->tileHeight
                        ));
                    }
                }
            }
            continue; // Saltar al siguiente layer
        }

        Layer layer;
        layer.vertices.setPrimitiveType(sf::Quads);
        layer.vertices.resize(m_mapWidth * m_mapHeight * 4);

        for (int y = 0; y < m_mapHeight; ++y) {
            for (int x = 0; x < m_mapWidth; ++x) {
                int tileNumber = tiles[y * m_mapWidth + x];
                if (tileNumber == 0) continue;

                TileSet* tileset = isDecoLayer ? decoTileset : mainTileset;
                if (!tileset) continue;

                int relativeTileID = tileNumber - tileset->firstGid;
                int tu = relativeTileID % tileset->columns;
                int tv = relativeTileID / tileset->columns;

                sf::Vertex* quad = &layer.vertices[(x + y * m_mapWidth) * 4];

                quad[0].position = sf::Vector2f(x * tileset->tileWidth, y * tileset->tileHeight);
                quad[1].position = sf::Vector2f((x + 1) * tileset->tileWidth, y * tileset->tileHeight);
                quad[2].position = sf::Vector2f((x + 1) * tileset->tileWidth, (y + 1) * tileset->tileHeight);
                quad[3].position = sf::Vector2f(x * tileset->tileWidth, (y + 1) * tileset->tileHeight);

                quad[0].texCoords = sf::Vector2f(tu * tileset->tileWidth, tv * tileset->tileHeight);
                quad[1].texCoords = sf::Vector2f((tu + 1) * tileset->tileWidth, tv * tileset->tileHeight);
                quad[2].texCoords = sf::Vector2f((tu + 1) * tileset->tileWidth, (tv + 1) * tileset->tileHeight);
                quad[3].texCoords = sf::Vector2f(tu * tileset->tileWidth, (tv + 1) * tileset->tileHeight);
            }
        }

        layer.tilesetTexture = isDecoLayer ? &decoTileset->texture : &mainTileset->texture;
        m_layers.push_back(layer);
    }

    return true;
}


bool TileMap::isColliding(const sf::FloatRect& playerBounds) const {
    for (const auto& block : collisionBlocks) {
        if (playerBounds.intersects(block)) {
            return true;
        }
    }
    return false;
}

sf::Vector2f TileMap::getSpawnPosition(float x, float y) const {
    // Creamos un rectángulo para el jugador en la posición solicitada
    sf::FloatRect playerBounds(x, y, 30, 30); // Tamaño aproximado del jugador (ajustar según necesidad)
    
    // Verificamos si la posición está libre de colisiones
    if (!isColliding(playerBounds)) {
        return sf::Vector2f(x, y); // Devolver la posición original si es válida
    }
    
    // Si hay colisión, intentamos encontrar una posición cercana libre
    const int MAX_ATTEMPTS = 100;
    const float SEARCH_RADIUS = 10.0f;
    
    for (int attempt = 1; attempt <= MAX_ATTEMPTS; ++attempt) {
        // Crear un patrón de búsqueda en espiral
        int offsetX = 0, offsetY = 0;
        int layer = 1;
        int leg = 1;
        
        for (int i = 0; i < attempt; ++i) {
            // Mover en la dirección actual
            switch ((i / leg) % 4) {
                case 0: offsetX += SEARCH_RADIUS; break; // Derecha
                case 1: offsetY += SEARCH_RADIUS; break; // Abajo
                case 2: offsetX -= SEARCH_RADIUS; break; // Izquierda
                case 3: offsetY -= SEARCH_RADIUS; break; // Arriba
            }
            
            // Aumentar el tamaño de la "pierna" cuando sea necesario
            if (i % (leg * 2) == 0) {
                layer++;
                leg = layer;
            }
        }
        
        // Comprobar la nueva posición
        sf::FloatRect newBounds(x + offsetX, y + offsetY, 30, 30);
        if (!isColliding(newBounds)) {
            return sf::Vector2f(x + offsetX, y + offsetY);
        }
    }
    
    // Si no encontramos una posición válida después de varios intentos,
    // buscamos en el mapa entero una posición sin colisión
    for (int tileY = 0; tileY < m_mapHeight; ++tileY) {
        for (int tileX = 0; tileX < m_mapWidth; ++tileX) {
            float testX = tileX * 32.0f; // Suponiendo que un tile es 32x32
            float testY = tileY * 32.0f;
            
            sf::FloatRect testBounds(testX, testY, 30, 30);
            if (!isColliding(testBounds)) {
                return sf::Vector2f(testX, testY);
            }
        }
    }
    
    // Si todo falla, devolvemos la posición original
    std::cerr << "No se pudo encontrar una posición de spawn válida." << std::endl;
    return sf::Vector2f(x, y);
}

// Implementación de la función `draw`, necesaria para evitar errores de vtable
void TileMap::draw(GameEngine& engine) const {
    for (const auto& layer : m_layers) {
        if (layer.tilesetTexture) {
            engine.drawVertices(layer.vertices, *layer.tilesetTexture, getTransform());
        }
    }
}

bool TileMap::isPlayerInteractingWithTile(const sf::FloatRect& playerBounds, int& outTileId) const {
    for (const auto& tile : interactiveTiles) {
        if (playerBounds.intersects(tile.rect)) {
            outTileId = tile.id;
            return true;
        }
    }
    return false;
}

void TileMap::addInteractiveTile(int id, const sf::FloatRect& rect) {
    interactiveTiles.push_back({id, rect});
}