#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <DetectedArea.h>
#include <DetectingArea.h>

#define kVel 5


// clase singleton para la lista dinamica usada en el main
// práctica para aprender a hacer un singleton.
class DataSingleton {
private:
    std::vector<DetectedArea *> inGameAreas;
    DataSingleton() {}

public:
    static DataSingleton &getInstance() {
        static DataSingleton instance;
        return instance;
    }

    void addArea(DetectedArea *area) {
        inGameAreas.push_back(area);
    }

    void removeArea(DetectedArea *area) {
        auto it = std::find(inGameAreas.begin(), inGameAreas.end(), area);
        if (it != inGameAreas.end()) {
            delete *it;
            inGameAreas.erase(it);
        }
    }

    std::vector<DetectedArea *> &getAreas() {
        return inGameAreas;
    }
};

class InteractionArea : public DetectingArea {
public:
    InteractionArea(float radius) : DetectingArea(radius) {
        setFillColor(sf::Color(255, 0, 0, 76));
    }

    bool isDetected(const DetectedArea &area) const override {
        return getGlobalBounds().intersects(area.getGlobalBounds());
    }

    void interact() override {
        std::cout << "Tamaño de detectedObjects: " << detectedObjects.size() << std::endl;
        if (!detectedObjects.empty()) {
            sortDetectedObjects();
            DetectedArea *area = detectedObjects.front();
            DataSingleton::getInstance().removeArea(area);
            detectedObjects.erase(detectedObjects.begin());
            std::cout << " He eliminado y liberado memoria." << std::endl;
        }
    }
};

class Jugador {
private:
    sf::Sprite sprite;
    InteractionArea interactionArea;

public:
    Jugador(const sf::Texture &tex) : sprite(tex), interactionArea(90.f) {
        sprite.setOrigin(75 / 2, 75 / 2);
        sprite.setTextureRect(sf::IntRect(0 * 75, 0 * 75, 75, 75));
        sprite.setPosition(320, 240);
        interactionArea.setOrigin(sf::Vector2f(90.f, 90.f));
        interactionArea.setPosition(sprite.getPosition());
        interactionArea.setFillColor(sf::Color(0, 128, 255, 76));
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
        case sf::Keyboard::Space:
            interactionArea.interact();
        default:
            return;
        }
        interactionArea.setPosition(sprite.getPosition());
    }

    void drawPlayer(sf::RenderWindow &window) {
        window.draw(interactionArea);
        window.draw(sprite);
    }

    void updatePlayerAreas() {
        interactionArea.detect(DataSingleton::getInstance().getAreas());
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
    DataSingleton &data = DataSingleton::getInstance();

    data.addArea(new DetectedArea(30.0f));
    data.getAreas().back()->setOrigin(30.f, 30.f);
    data.getAreas().back()->setPosition(400, 300);

    data.addArea(new DetectedArea(30.0f));
    data.getAreas().back()->setOrigin(30.0f, 30.0f);
    data.getAreas().back()->setPosition(500, 300);

    data.addArea(new DetectedArea(30.0f));
    data.getAreas().back()->setOrigin(30.0f, 30.0f);
    data.getAreas().back()->setPosition(500, 400);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed)
                jugador.mover(event.key.code);
        }

        jugador.updatePlayerAreas();

        window.clear();
        for (DetectedArea *area : data.getAreas()) {
            if (area != nullptr)
                window.draw(*area);
        }
        jugador.drawPlayer(window);
        window.display();
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