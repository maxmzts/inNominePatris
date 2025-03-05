// detectedArea.h
#pragma once
#include <SFML/Graphics.hpp>

// Clase abstracta que representa un área detectable en el juego
class DetectedArea : public sf::CircleShape {
public:
    DetectedArea(float radius);
    virtual ~DetectedArea() = default;
};
