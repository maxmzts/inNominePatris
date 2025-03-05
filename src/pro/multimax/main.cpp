#include <SFML/Graphics.hpp>
#include <iostream>
#include <DetectedArea.h>
#include <DetectingArea.h>

#define kVel 5

class Jugador {
private:
    sf::Sprite sprite;
    DetectedArea detectedArea;

public:
    Jugador(const sf::Texture &tex) : sprite(tex), detectedArea(45.0f) {
        sprite.setOrigin(75 / 2, 75 / 2);
        sprite.setTextureRect(sf::IntRect(0 * 75, 0 * 75, 75, 75));
        sprite.setPosition(320, 240);
        
        detectedArea.setOrigin(45.0f, 45.0f);
        detectedArea.setPosition(sprite.getPosition());
        detectedArea.setFillColor(sf::Color(0,128,255,76));
    }

    void mover(sf::Keyboard::Key key) {
        switch (key) {
            case sf::Keyboard::Right:
                sprite.setTextureRect(sf::IntRect(0 * 75, 2 * 75, 75, 75));
                sprite.setScale(1, 1);
                sprite.move(kVel, 0);
                break;
            case sf::Keyboard::Left:
                sprite.setTextureRect(sf::IntRect(0 * 75, 2 * 75, 75, 75));
                sprite.setScale(-1, 1);
                sprite.move(-kVel, 0);
                break;
            case sf::Keyboard::Up:
                sprite.setTextureRect(sf::IntRect(0 * 75, 3 * 75, 75, 75));
                sprite.move(0, -kVel);
                break;
            case sf::Keyboard::Down:
                sprite.setTextureRect(sf::IntRect(0 * 75, 0 * 75, 75, 75));
                sprite.move(0, kVel);
                break;
            default:
                return;
        }
        detectedArea.setPosition(sprite.getPosition());
    }

    void drawPlayer(sf::RenderWindow &window) {
        window.draw(detectedArea);
        window.draw(sprite);
    }

    DetectedArea& getDetectedArea() { return detectedArea; }
};

class DetectingAreaImpl : public DetectingArea {
    public:
        DetectingAreaImpl(float radius) : DetectingArea(radius) {
            setFillColor(sf::Color(255, 0, 0, 76)); // Rojo con 30% de opacidad
        }
    
        bool isDetected(const DetectedArea& area) const override {
            return getGlobalBounds().intersects(area.getGlobalBounds());
        }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(640, 480), "P0. Fundamentos de los Videojuegos. DCCIA");
    sf::Texture tex;
    if (!tex.loadFromFile("resources/sprites.png")) {
        std::cerr << "Error cargando la imagen sprites.png";
        return 1;
    }

    Jugador jugador(tex);
    DetectingAreaImpl detectingArea(90.0f);
    detectingArea.setOrigin(90.0f, 90.0f);
    detectingArea.setPosition(400, 300);
    int count = 0;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed)
                jugador.mover(event.key.code);
        }

        detectingArea.detect({ &jugador.getDetectedArea() });
        if (!detectingArea.getDetectedArea().empty()) {
            std::cout << "Estoy dentro " << count << std::endl;
        }
        
        window.clear();
        window.draw(detectingArea);
        jugador.drawPlayer(window);
        window.display();
        count++;
    }
    return 0;
}


/* #include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

#include "game_class/Game.h"
#include "include/config.h"

int main() {
    //init srand
    std::srand(static_cast<unsigned>(time(NULL)));

    //Inicializar motor de juego
    Game game;
  
    //Bucle del juego
    while (game.running()) {

        //actualizar
        game.update();
        
        //renderizar
        game.render();
    }

  return 0;
}
*/