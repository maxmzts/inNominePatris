#include "VFXManager.h"

void VFXManager::addEffect(
    const std::string& texturePath,
    sf::Vector2f position,
    sf::Vector2i frameSize,
    int frameCount,
    float frameDuration
) {
    effects.emplace_back(std::make_unique<VisualEffect>(
        texturePath, position, frameSize, frameCount, frameDuration
    ));
}

void VFXManager::update(float deltaTime) {
    for (auto& effect : effects) {
        effect->update(deltaTime);
    }

    // Eliminar los efectos terminados
    effects.erase(
        std::remove_if(effects.begin(), effects.end(),
            [](const std::unique_ptr<VisualEffect>& e) {
                return e->isFinished();
            }),
        effects.end()
    );
}

void VFXManager::draw(sf::RenderWindow& window) {
    for (const auto& effect : effects) {
        effect->draw(window);
    }
}
