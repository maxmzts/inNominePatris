#include "TileMap.h"
#include <iostream>
#include <sstream>

bool TileMap::loadFromFile(const std::string& filename) {
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filename.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Error cargando el mapa XML\n";
        return false;
    }

    // Obtener nodo raíz <map>
    tinyxml2::XMLElement* mapElement = doc.FirstChildElement("map");
    if (!mapElement) return false;

    mapElement->QueryIntAttribute("width", &m_mapWidth);
    mapElement->QueryIntAttribute("height", &m_mapHeight);

    // Leer tilesets
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
        tileset.texture.loadFromFile(std::string("resources/") + source);
        tilesetElement->QueryIntAttribute("tilewidth", &tileset.tileWidth);
        tilesetElement->QueryIntAttribute("tileheight", &tileset.tileHeight);
        tileset.columns = tileset.texture.getSize().x / tileset.tileWidth;

        m_tilesets[firstGid] = tileset;
    }

    // Leer capas
    for (tinyxml2::XMLElement* layerElement = mapElement->FirstChildElement("layer");
         layerElement; layerElement = layerElement->NextSiblingElement("layer")) {
        
        tinyxml2::XMLElement* dataElement = layerElement->FirstChildElement("data");
        if (!dataElement) continue;

        std::vector<int> tiles;
        const char* csvData = dataElement->GetText();
        std::stringstream ss(csvData);
        std::string tileID;
        
        while (std::getline(ss, tileID, ',')) {
            tiles.push_back(std::stoi(tileID));
        }

        // Crear una nueva capa
        Layer layer;
        layer.vertices.setPrimitiveType(sf::Quads);
        layer.vertices.resize(m_mapWidth * m_mapHeight * 4);

        for (int y = 0; y < m_mapHeight; ++y) {
            for (int x = 0; x < m_mapWidth; ++x) {
                int tileNumber = tiles[y * m_mapWidth + x];
                if (tileNumber == 0) continue; // Espacio vacío

                // Determinar qué tileset usar
                TileSet* tileset = nullptr;
                for (auto& [firstGid, ts] : m_tilesets) {
                    if (tileNumber >= firstGid) {
                        tileset = &ts;
                    } else {
                        break;
                    }
                }
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

        // Asignar textura de tileset a la capa
        if (!m_tilesets.empty()) {
            layer.tilesetTexture = &m_tilesets.begin()->second.texture;
        }

        m_layers.push_back(layer);
    }

    return true;
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();

    for (const auto& layer : m_layers) {
        states.texture = layer.tilesetTexture;
        target.draw(layer.vertices, states);
    }
}
