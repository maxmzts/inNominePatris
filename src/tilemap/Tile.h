#ifndef TILE_H
#define TILE_H

#include <SFML/Graphics.hpp>

// Representa un tile individual
struct Tile {
    int id;                 // ID del tile en el mapa lógico
    sf::IntRect textureRect; // Área del tileset que representa este tile
    sf::Vector2u size;       // Tamaño del tile en píxeles

    Tile(int id, sf::IntRect rect, sf::Vector2u size)
        : id(id), textureRect(rect), size(size) {}
};

#endif
