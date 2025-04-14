#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "SoundEffect.h"

/**
 * Manager planteado para gestionar los efectos visuales del juego.
 * Se plantea como un singleton para que se gestione todo el VFX en
 * un único lugar pero que de igual manera se pueda acceder desde 
 * distintos sitios.
 * Su diseño depende directamente del diseño de "VisualEffect".
 */
class SFXManager {
private:
    std::vector<std::unique_ptr<SoundEffect>> effects;
    SFXManager() = default;
    SFXManager(const SFXManager&) = delete;
    SFXManager& operator=(const SFXManager&) = delete;
public:
    static SFXManager& getInstance();

    void addEffect(
        const std::string& soundPath,
        float volume = 100.f, 
        float pitch = 1.f
    );

    void update(float deltaTime);
};

