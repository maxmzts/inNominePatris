#include "VFXManager.h"

VFXManager& VFXManager::getInstance() {
    static VFXManager instance;
    return instance;
}

void VFXManager::addEffect(
    const std::string& texturePath,
    sf::Vector2f position,
    sf::Vector2i frameSize,
    int frameCount,
    float framePerSecond,
    bool invert,
    bool loop
) {
    effects.emplace_back(std::make_unique<VisualEffect>(
        texturePath, position, frameSize, frameCount, framePerSecond, invert, loop
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

void VFXManager::render(sf::RenderWindow& window) {
    for (const auto& effect : effects) {
        effect->draw(window);
    }
}
