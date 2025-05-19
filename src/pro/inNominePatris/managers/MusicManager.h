#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "MusicTrack.h"

/**
 * Manager planteado para gestionar los efectos visuales del juego.
 * Se plantea como un singleton para que se gestione todo el VFX en
 * un único lugar pero que de igual manera se pueda acceder desde 
 * distintos sitios.
 * Su diseño depende directamente del diseño de "VisualEffect".
 */
class MusicManager {
private:
    struct TrackTransition {
        std::unique_ptr<MusicTrack> track;
        float targetVolume;
        float initialVolume;
        float transitionDuration;
        float transitionTime;
        bool fadeOut;
    };

    std::vector<TrackTransition> activeTransitions;
    std::vector<std::unique_ptr<MusicTrack>> tracks;
    MusicManager() = default;
    MusicManager(const MusicManager&) = delete;
    MusicManager& operator=(const MusicManager&) = delete;

public:
    static MusicManager& getInstance();
    void addTrack(
        const std::string& soundPath,
        float volume = 100.f,
        bool loop = true,
        float pitch = 1.f
    );
    void update(float deltaTime);
    void transitionTo(
        const std::string& soundPath,
        float volume = 100.f,
        float pitch = 1.f,
        float transitionDuration = 2.0f
    );
    void clear(); // Detiene y elimina todas las pistas de música
};

