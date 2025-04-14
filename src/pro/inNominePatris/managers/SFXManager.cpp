#include "SFXManager.h"

SFXManager& SFXManager::getInstance() {
    static SFXManager instance;
    return instance;
}

void SFXManager::addEffect(
    const std::string& texturePath,
    float volume, 
    float pitch
) {
    effects.emplace_back(std::make_unique<SoundEffect>(
        texturePath, volume, pitch
    ));
}

void SFXManager::update(float deltaTime) {
    // Eliminar los efectos terminados
    effects.erase(
        std::remove_if(effects.begin(), effects.end(),
            [](const std::unique_ptr<SoundEffect>& e) {
                return e->isStopped();
            }),
        effects.end()
    );
}
