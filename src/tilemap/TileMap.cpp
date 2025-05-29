#include "TileMap.h"
#include <iostream>
#include <sstream>

bool TileMap::loadFromFile(const std::string& filename, GameEngine& engine) {
    // Limpiar datos anteriores antes de cargar el nuevo mapa
    clear();

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
        tileset.texture = engine.loadTexture(std::string("./src/resources/") + source); // Usar el motor para cargar texturas
        tilesetElement->QueryIntAttribute("tilewidth", &tileset.tileWidth);
        tilesetElement->QueryIntAttribute("tileheight", &tileset.tileHeight);
        tileset.columns = tileset.texture.getSize().x / tileset.tileWidth;

        // std::cout << "Cargado tileset con firstGid=" << firstGid 
                //   << ", tileWidth=" << tileset.tileWidth 
                //   << ", tileHeight=" << tileset.tileHeight 
                //   << ", columns=" << tileset.columns << std::endl;

        m_tilesets[firstGid] = tileset;

        if (!mainTileset) {
            mainTileset = &m_tilesets[firstGid];
        } else {
            decoTileset = &m_tilesets[firstGid];
        }
    }

    int layerIndex = 0;
    
    for (tinyxml2::XMLElement* layerElement = mapElement->FirstChildElement("layer");
         layerElement; layerElement = layerElement->NextSiblingElement("layer")) {
        
        const char* layerName = layerElement->Attribute("name");
        if (!layerName) continue;
        
        std::string layerNameStr(layerName);
        bool isBoundsLayer = (layerNameStr == "bounds");
        bool isInteractiveLayer = (layerNameStr == "interaction");

        // std::cout << "Procesando capa: " << layerNameStr << std::endl;

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
            // Guardar los tiles de la capa de colisiones
            m_boundsTiles = tiles;
            
            // Actualizar los bloques de colisión
            updateCollisionBlocks(mainTileset);
            
            // Registramos el bounds layer en m_layerIndices con un índice especial, por ejemplo -1
            // para indicar que es un caso especial que no está en m_layers
            m_layerIndices[layerNameStr] = -1;
            // std::cout << "Capa de bounds registrada con índice especial -1" << std::endl;
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
            
            // Registramos el interactive layer en m_layerIndices con un índice especial, por ejemplo -2
            m_layerIndices[layerNameStr] = -2;
            // std::cout << "Capa de interacción registrada con índice especial -2" << std::endl;
            continue; // Saltar al siguiente layer
        }

        Layer layer;
        layer.vertices.setPrimitiveType(sf::Quads);
        layer.vertices.resize(m_mapWidth * m_mapHeight * 4);
        layer.tiles = tiles; // Guardar los IDs de los tiles
        layer.name = layerNameStr; // Guardar el nombre de la capa
        
        // Determinar qué tileset usar para esta capa
        bool isDecoLayer = (layerNameStr == "deco");
        TileSet* tilesetToUse = isDecoLayer ? decoTileset : mainTileset;
        
        // Poblar los vértices para renderizado
        updateLayerVertices(layer, tilesetToUse);

        layer.tilesetTexture = tilesetToUse ? &tilesetToUse->texture : nullptr;
        layer.tileset = tilesetToUse;
        
        // Guardar el nombre de la capa y su índice DESPUÉS de añadir la capa a m_layers
        m_layerIndices[layerNameStr] = layerIndex;
        m_layers.push_back(layer);
        
        // std::cout << "Capa '" << layerNameStr << "' registrada con índice " << layerIndex 
        //          << ", usando tileset con firstGid=" << (tilesetToUse ? tilesetToUse->firstGid : 0) << std::endl;
        
        layerIndex++;
    }

    return true;
}

void TileMap::updateLayerVertices(Layer& layer, TileSet* tileset) {
    if (!tileset) return;
    
    // std::cout << "Actualizando vértices para la capa '" << layer.name 
    //          << "' con tileset firstGid=" << tileset->firstGid << std::endl;
    
    layer.vertices.clear();
    layer.vertices.setPrimitiveType(sf::Quads);
    layer.vertices.resize(m_mapWidth * m_mapHeight * 4);

    for (int y = 0; y < m_mapHeight; ++y) {
        for (int x = 0; x < m_mapWidth; ++x) {
            int tileNumber = layer.tiles[y * m_mapWidth + x];
            if (tileNumber == 0) continue;

            int relativeTileID = tileNumber - tileset->firstGid;
            // if (relativeTileID < 0) {
            //     std::cerr << "ERROR: TileID " << tileNumber << " menor que firstGid " 
            //               << tileset->firstGid << " en capa '" << layer.name 
            //               << "' en posición (" << x << "," << y << ")" << std::endl;
            //     continue;
            // }
            
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
}

void TileMap::updateSingleTile(Layer& layer, int x, int y, int oldTileId, int newTileId) {
    if (!layer.tileset) {
        std::cerr << "updateSingleTile: No hay tileset asociado a la capa '" << layer.name << "'" << std::endl;
        return;
    }
    
    // Índice del quad en el array de vértices
    int quadIndex = (x + y * m_mapWidth) * 4;
    
    // std::cout << "Actualizando tile en (" << x << "," << y << ") de ID=" << oldTileId 
            //   << " a ID=" << newTileId << " en capa '" << layer.name 
            //   << "' (tileset firstGid=" << layer.tileset->firstGid << ")" << std::endl;
    
    // Si el nuevo ID es 0, hacemos el quad invisible estableciendo alfa a 0
    if (newTileId == 0) {
        for (int i = 0; i < 4; i++) {
            if (quadIndex + i < layer.vertices.getVertexCount()) {
                layer.vertices[quadIndex + i].color.a = 0;
            }
        }
        // std::cout << "Tile establecido como invisible (ID=0)" << std::endl;
        return;
    }
    
    // Verificar que el newTileId sea válido para este tileset
    if (newTileId < layer.tileset->firstGid) {
        std::cerr << "ERROR: Tile ID " << newTileId << " inválido para este tileset (firstGid = " 
                  << layer.tileset->firstGid << ")" << std::endl;
        return;
    }
    
    // Si el quad no existe o estaba vacío (ID = 0), necesitamos crear/restaurar el quad
    bool needsFullUpdate = (oldTileId == 0 || quadIndex + 3 >= layer.vertices.getVertexCount());
    
    // Si necesitamos una actualización completa, aseguramos que el array tenga el tamaño correcto
    if (needsFullUpdate) {
        // Si el array de vértices no es lo suficientemente grande, ajustamos su tamaño
        if (layer.vertices.getVertexCount() < (m_mapWidth * m_mapHeight * 4)) {
            // std::cout << "Redimensionando array de vértices a " << (m_mapWidth * m_mapHeight * 4) << std::endl;
            layer.vertices.resize(m_mapWidth * m_mapHeight * 4);
        }
    }
    
    // Calcular las coordenadas de textura para el nuevo tile
    int relativeTileID = newTileId - layer.tileset->firstGid;
    int tu = relativeTileID % layer.tileset->columns;
    int tv = relativeTileID / layer.tileset->columns;
    
    // std::cout << "relativeTileID=" << relativeTileID << ", tu=" << tu << ", tv=" << tv 
    //          << ", columns=" << layer.tileset->columns << std::endl;
    
    // Actualizar las posiciones y coordenadas de textura del quad
    sf::Vertex* quad = &layer.vertices[quadIndex];
    
    // Posiciones de los vértices
    quad[0].position = sf::Vector2f(x * layer.tileset->tileWidth, y * layer.tileset->tileHeight);
    quad[1].position = sf::Vector2f((x + 1) * layer.tileset->tileWidth, y * layer.tileset->tileHeight);
    quad[2].position = sf::Vector2f((x + 1) * layer.tileset->tileWidth, (y + 1) * layer.tileset->tileHeight);
    quad[3].position = sf::Vector2f(x * layer.tileset->tileWidth, (y + 1) * layer.tileset->tileHeight);
    
    // Coordenadas de textura
    quad[0].texCoords = sf::Vector2f(tu * layer.tileset->tileWidth, tv * layer.tileset->tileHeight);
    quad[1].texCoords = sf::Vector2f((tu + 1) * layer.tileset->tileWidth, tv * layer.tileset->tileHeight);
    quad[2].texCoords = sf::Vector2f((tu + 1) * layer.tileset->tileWidth, (tv + 1) * layer.tileset->tileHeight);
    quad[3].texCoords = sf::Vector2f(tu * layer.tileset->tileWidth, (tv + 1) * layer.tileset->tileHeight);
    
    // Restaurar el color para asegurar que es visible
    for (int i = 0; i < 4; i++) {
        quad[i].color = sf::Color::White;
    }
    
    // std::cout << "Vértices actualizados correctamente para el nuevo tile" << std::endl;
}


bool TileMap::isColliding(const sf::FloatRect& playerBounds) const {
    for (const auto& block : collisionBlocks) {
        if (block.rect.intersects(playerBounds)) {
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
    // std::cout << "Añadido tile interactivo con ID=" << id 
    //          << " en posición (" << rect.left << "," << rect.top << ")" << std::endl;
}


// PARA CAMBIO Y ELMINACION DE TILES
// Modificación para setLocalTile
bool TileMap::setLocalTile(const std::string& layerName, int x, int y, int localTileId) {
    // Caso especial para "bounds" - enviarlo directamente a setTile
    if (layerName == "bounds") {
        // Para valores negativos (-1, -2, etc.), establecemos el ID a 0
        // lo que eliminará la colisión en updateCollisionBlocks()
        int globalTileId = (localTileId < 0) ? 0 : localTileId;
        return setTile(layerName, x, y, globalTileId);
    }

    if (layerName == "interaction") {
        int globalTileId = (localTileId < 0) ? 0 : localTileId;
        return setTile(layerName, x, y, globalTileId);
    }
    
    // Para otras capas
    auto layerIndexIt = m_layerIndices.find(layerName);
    if (layerIndexIt == m_layerIndices.end() || layerIndexIt->second < 0 || layerIndexIt->second >= m_layers.size()) {
        std::cerr << "setLocalTile: No se encontró la capa '" << layerName << "'" << std::endl;
        return false;
    }
    
    // Obtener la capa y su tileset
    Layer& layer = m_layers[layerIndexIt->second];
    if (!layer.tileset) {
        std::cerr << "setLocalTile: No hay tileset asociado a la capa '" << layerName << "'" << std::endl;
        return false;
    }
    
    // Convertir ID local a ID global
    int globalTileId = (localTileId >= 0) ? (layer.tileset->firstGid + localTileId) : 0;
    
    // Usar la función existente para establecer el tile
    return setTile(layerName, x, y, globalTileId);
}

// Modificación para setTile
bool TileMap::setTile(const std::string& layerName, int x, int y, int tileId) {
    // Verificar que las coordenadas estén dentro del mapa
    if (x < 0 || x >= m_mapWidth || y < 0 || y >= m_mapHeight) {
        std::cerr << "setTile: Coordenadas (" << x << ", " << y << ") fuera del mapa." << std::endl;
        return false;
    }
    
    // std::cout << "Estableciendo tile en capa '" << layerName << "' en (" << x << "," << y 
    //          << ") con ID=" << tileId << std::endl;
    
    // Caso especial para la capa de colisiones
    if (layerName == "bounds") {
        // Verificar que tengamos datos de bounds
        if (m_boundsTiles.size() != m_mapWidth * m_mapHeight) {
            std::cerr << "setTile: Datos de bounds no inicializados correctamente." << std::endl;
            return false;
        }
        
        // Actualizar el tile de colisión
        m_boundsTiles[y * m_mapWidth + x] = tileId;
        
        // Obtener el tileset principal
        TileSet* mainTileset = nullptr;
        if (!m_tilesets.empty()) {
            mainTileset = &m_tilesets.begin()->second;
        }
        
        if (!mainTileset) {
            std::cerr << "setTile: No se encontró tileset para bounds." << std::endl;
            return false;
        }
        
        // Actualizar las colisiones
        updateCollisionBlocks(mainTileset);
        // std::cout << "Tile de colisión actualizado en (" << x << ", " << y << ") a ID " << tileId << std::endl;
        return true;
    }
    
    // Caso especial para la capa de interacción
    if (layerName == "interaction") {
        TileSet* mainTileset = nullptr;
        if (!m_tilesets.empty()) {
            mainTileset = &m_tilesets.begin()->second;
        }
        
        if (!mainTileset) {
            std::cerr << "setTile: No se encontró tileset para interaction." << std::endl;
            return false;
        }
        
        float worldX = x * mainTileset->tileWidth;
        float worldY = y * mainTileset->tileHeight;
        
        // Eliminar cualquier tile interactivo en esta posición
        auto it = interactiveTiles.begin();
        while (it != interactiveTiles.end()) {
            if (it->rect.left == worldX && it->rect.top == worldY) {
                it = interactiveTiles.erase(it);
            } else {
                ++it;
            }
        }
        
        // Añadir nuevo tile interactivo si tileId != 0
        if (tileId != 0) {
            addInteractiveTile(tileId, sf::FloatRect(
                worldX, worldY,
                mainTileset->tileWidth, mainTileset->tileHeight
            ));
        }
        
        // std::cout << "Tile de interacción actualizado en (" << x << ", " << y << ") a ID " << tileId << std::endl;
        return true;
    }
    
    // Para otras capas, buscar por nombre
    auto layerIndexIt = m_layerIndices.find(layerName);
    if (layerIndexIt == m_layerIndices.end()) {
        std::cerr << "setTile: No se encontró la capa '" << layerName << "'" << std::endl;
        return false;
    }
    
    int layerIndex = layerIndexIt->second;
    // std::cout << "Índice de capa encontrado: " << layerIndex << std::endl;
    
    // Caso normal para capas regulares
    if (layerIndex < 0 || layerIndex >= m_layers.size()) {
        std::cerr << "setTile: Índice de capa fuera de rango: " << layerIndex << " (m_layers.size = " 
                  << m_layers.size() << ")" << std::endl;
        return false;
    }
    
    // Obtener la capa
    Layer& layer = m_layers[layerIndex];
    
    // Verificar que tengamos datos de tiles
    if (layer.tiles.size() != m_mapWidth * m_mapHeight) {
        std::cerr << "setTile: Datos de tiles no inicializados correctamente para la capa '" 
                  << layerName << "'" << std::endl;
        return false;
    }
    
    // Obtener el tileset asociado a esta capa
    if (!layer.tileset) {
        std::cerr << "setTile: No hay tileset asociado a la capa '" << layerName << "'" << std::endl;
        return false;
    }
    
    // Actualizar el ID del tile
    int tileIndex = y * m_mapWidth + x;
    int oldTileId = layer.tiles[tileIndex];
    layer.tiles[tileIndex] = tileId;
    
    // Actualizar solo el quad específico en lugar de regenerar todos los vértices
    updateSingleTile(layer, x, y, oldTileId, tileId);
    
    // std::cout << "Tile actualizado en capa '" << layerName << "' en (" 
    //          << x << ", " << y << ") de ID " << oldTileId << " a ID " << tileId << std::endl;
    return true;
}



void TileMap::updateCollisionBlocks(TileSet* tileset) {
    if (!tileset) return;
    
    // std::cout << "Actualizando bloques de colisión..." << std::endl;
    
    collisionBlocks.clear();
    
    if (m_boundsTiles.empty()) return;
    
    for (int y = 0; y < m_mapHeight; ++y) {
        for (int x = 0; x < m_mapWidth; ++x) {
            int tileId = m_boundsTiles[y * m_mapWidth + x];
            // Solo añadir colisión para tiles con ID > 0 (positivos)
            if (tileId > 0) {
                CollisionBlock block;
                block.rect = sf::FloatRect(
                    x * tileset->tileWidth, 
                    y * tileset->tileHeight,
                    tileset->tileWidth, 
                    tileset->tileHeight
                );
                block.tileId = tileId;
                block.x = x;
                block.y = y;
                collisionBlocks.push_back(block);
            }
        }
    }
    
    // std::cout << "Total de bloques de colisión: " << collisionBlocks.size() << std::endl;
}

bool TileMap::removeCollisionByCoord(int x, int y) {
    auto it = std::remove_if(collisionBlocks.begin(), collisionBlocks.end(),
        [x, y](const CollisionBlock& block) {
            return block.x == x && block.y == y;
        });
    // std::cout << std::endl;
    
    bool removed = it != collisionBlocks.end();
    collisionBlocks.erase(it, collisionBlocks.end());
    if (removed) {
        // También actualizar m_boundsTiles para reflejar la eliminación
        m_boundsTiles[y * m_mapWidth + x] = 0;
        // std::cout << "Eliminado bloque de colisión en (" << x << ", " << y << ")" << std::endl;
    }
    
    return removed;
}

bool TileMap::removeCollisionByTileId(int tileId) {
    bool anyRemoved = false;
    
    auto it = std::remove_if(collisionBlocks.begin(), collisionBlocks.end(),
        [tileId, this, &anyRemoved](const CollisionBlock& block) {
            if (block.tileId == tileId) {
                // Actualizar m_boundsTiles cuando encontramos un bloque a eliminar
                m_boundsTiles[block.y * m_mapWidth + block.x] = 0;
                anyRemoved = true;
                return true;
            }
            return false;
        });
    
    collisionBlocks.erase(it, collisionBlocks.end());
    
    if (anyRemoved) {
        // std::cout << "Eliminados bloques de colisión con ID de tile: " << tileId << std::endl;
    }
    
    return anyRemoved;
}

bool TileMap::toggleCollisionAt(int x, int y) {
    // Primero verificamos si ya hay un bloque en esa posición
    auto it = std::find_if(collisionBlocks.begin(), collisionBlocks.end(),
        [x, y](const CollisionBlock& block) {
            return block.x == x && block.y == y;
        });
    
    if (it != collisionBlocks.end()) {
        // Si existe, lo eliminamos
        collisionBlocks.erase(it);
        m_boundsTiles[y * m_mapWidth + x] = 0;
        // std::cout << "Desactivada colisión en (" << x << ", " << y << ")" << std::endl;
        return true;
    } else {
        // Si no existe, verificamos si hay un tile en la capa principal
        if (m_layers.empty() || m_tilesets.empty()) return false;
        
        // Buscar un tileset válido
        TileSet* mainTileset = nullptr;
        if (!m_tilesets.empty()) {
            mainTileset = &(m_tilesets.begin()->second);
        }
        
        if (!mainTileset) return false;
        
        // Añadir un nuevo bloque de colisión
        CollisionBlock block;
        block.rect = sf::FloatRect(
            x * mainTileset->tileWidth, 
            y * mainTileset->tileHeight,
            mainTileset->tileWidth, 
            mainTileset->tileHeight
        );
        block.tileId = 1; // Usamos un ID genérico para colisión
        block.x = x;
        block.y = y;
        
        collisionBlocks.push_back(block);
        // También actualizamos el array de tiles de colisión
        m_boundsTiles[y * m_mapWidth + x] = block.tileId;
        
        // std::cout << "Activada colisión en (" << x << ", " << y << ")" << std::endl;
        return true;
    }
}


void TileMap::clear() {
    // Limpiar todas las estructuras de datos
    m_layers.clear();
    m_tilesets.clear();
    interactiveTiles.clear();
    m_boundsTiles.clear();
    m_layerIndices.clear();
    collisionBlocks.clear();
    
    // Reiniciar dimensiones del mapa
    m_mapWidth = 0;
    m_mapHeight = 0;
    
    // std::cout << "Limpiados todos los datos del mapa anterior" << std::endl;
}