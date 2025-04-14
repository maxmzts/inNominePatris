#include "MusicManager.h"

MusicManager& MusicManager::getInstance() {
    static MusicManager instance;
    return instance;
}

void MusicManager::addTrack(
    const std::string& texturePath,
    float volume, 
    bool loop,
    float pitch
) {
    tracks.emplace_back(std::make_unique<MusicTrack>(
        texturePath, volume, pitch
    ));
}

void MusicManager::update(float deltaTime) {
    // Eliminar los efectos terminados
    tracks.erase(
        std::remove_if(tracks.begin(), tracks.end(),
            [](const std::unique_ptr<MusicTrack>& e) {
                return e->isStopped();
            }),
        tracks.end()
    );
}
