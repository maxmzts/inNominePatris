#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "VisualEffect.h"

/**
 * Manager planteado para gestionar los efectos visuales del juego.
 * Se plantea como un singleton para que se gestione todo el VFX en
 * un único lugar pero que de igual manera se pueda acceder desde 
 * distintos sitios.
 * Su diseño depende directamente del diseño de "VisualEffect".
 */
class VFXManager {
private:
    std::vector<std::unique_ptr<VisualEffect>> effects;
    VFXManager() = default;
    // No copy
    VFXManager(const VFXManager&) = delete;
    VFXManager& operator=(const VFXManager&) = delete;
public:
    static VFXManager& getInstance();

    void addEffect(
        const std::string& texturePath,
        sf::Vector2f position,
        sf::Vector2i frameSize,
        int frameCount,
        float frameDuration,
        bool invert = false,
        bool loop = false
    );

    void update(float deltaTime);
    void render(sf::RenderWindow& window);
};

