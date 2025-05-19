#include "SpriteFacade.h"

// Constructor vacío
SpriteFacade::SpriteFacade() = default;

// Constructor que carga una textura desde un archivo
SpriteFacade::SpriteFacade(const std::string& texturePath) {
    loadTexture(texturePath);
}

// Cargar textura desde un archivo
bool SpriteFacade::loadTexture(const std::string& texturePath) {
    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "Error: No se pudo cargar la textura desde " << texturePath << std::endl;
        return false;
    }
    sprite.setTexture(texture);
    return true;
}

// Establecer la posición del sprite
void SpriteFacade::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

void SpriteFacade::setRotation(float angle) {
    sprite.setRotation(angle);
}

void SpriteFacade::setTextureRect(sf::IntRect rect){
    sprite.setTextureRect(rect);
}

// Obtener la posición del sprite
sf::Vector2f SpriteFacade::getPosition() const {
    return sprite.getPosition();
}

// Establecer el origen del sprite
void SpriteFacade::setOrigin(float x, float y) {
    sprite.setOrigin(x, y);
}

// Escalar el sprite
void SpriteFacade::setScale(float scaleX, float scaleY) {
    sprite.setScale(scaleX, scaleY);
}

// Rotar el sprite
void SpriteFacade::rotate(float angle) {
    sprite.rotate(angle);
}

// Dibujar el sprite en una ventana
void SpriteFacade::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
}

// Obtener el tamaño global del sprite
sf::FloatRect SpriteFacade::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}

// Obtener el tamaño global del sprite
sf::FloatRect SpriteFacade::getLocalBounds() const {
    return sprite.getLocalBounds();
}

// Establecer el color del sprite
void SpriteFacade::setColor(const sf::Color& color) {
    sprite.setColor(color);
}

const sf::Sprite& SpriteFacade::getSprite() const {
    return sprite;
}