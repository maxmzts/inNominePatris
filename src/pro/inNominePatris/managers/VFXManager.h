#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "VisualEffect.h"

class VFXManager {
private:
    std::vector<std::unique_ptr<VisualEffect>> effects;
public:
    VFXManager() = default;

    void addEffect(
        const std::string& texturePath,
        sf::Vector2f position,
        sf::Vector2i frameSize,
        int frameCount,
        float frameDuration
    );

    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
};

