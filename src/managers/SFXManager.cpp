#include "SFXManager.h"
#include <iostream>

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

void SFXManager::update() {
    // Verificar si algún sonido ha terminado de reproducirse naturalmente
    for (auto& effect : effects) {
        if (effect->hasFinished()) {
            effect->stop(); // Actualizamos el estado interno a Stopped
        }
    }

    // std::cout << effects.size() << std::endl;
    
    // Eliminar los efectos terminados
    effects.erase(
        std::remove_if(effects.begin(), effects.end(),
            [](const std::unique_ptr<SoundEffect>& e) {
                return e->isStopped();
            }),
        effects.end()
    );
}
