#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class GameEngine {
public:
    // Constructor y destructor
    GameEngine(const std::string& title, int width, int height);
    ~GameEngine();

    // Métodos principales
    void run(); // Bucle principal del juego
    void drawSprite(const sf::Sprite& sprite); // Dibujar un sprite
    void clear(); // Limpiar la pantalla
    void display(); // Mostrar el contenido en pantalla

    // Métodos para manejar eventos
    bool isWindowOpen() const;
    sf::RenderWindow& getWindow(); // Acceso a la ventana (si es necesario)
    sf::Sprite createSprite(const std::string& texturePath, const sf::Vector2f& position); // Crear un sprite a partir de una textura

private:
    sf::RenderWindow window; // Ventana principal
    std::vector<sf::Sprite> spritesToDraw; // Lista de sprites a dibujar
};

#endif // GAME_ENGINE_H