#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

#include <DetectedArea.h>
#include <DetectingArea.h>

#include <Entity.h>
#include <Hitbox.h>
#include <Hurtbox.h>

#include <SoundPlayer.h>
#include <MusicPlayer.h>

#define kVel 5
#define ENEMY_SPEED  1

// clase singleton para la lista dinamica usada en el main
// práctica para aprender a hacer un singleton.
class DataSingleton {
private:
    std::vector<DetectedArea *> inGameAreas; //test areas
    DataSingleton() {}

public:
    SoundPlayer soundPlayer;
    sf::Vector2f playerPosition;
    std::vector<Enemigo> enemigos;

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

// IMPLEMENTACIÓN DE LA INTERFAZ DE DETECCION
class InteractionArea : public DetectingArea {
public:
    InteractionArea(float radius) : DetectingArea(radius) {
        setFillColor(sf::Color(255, 0, 0, 76));
    }

    void interact() override {
        if (!detectedObjects.empty()) {
            sortDetectedObjects();
            DetectedArea *area = detectedObjects.front();
            DataSingleton::getInstance().removeArea(area);
            detectedObjects.erase(detectedObjects.begin());
        }
    }
};

// CLASE DE JUGADOR PROVISIONAL PARA LAS PRUEBAS
class Jugador: public Entity {
private:
    sf::Sprite sprite;
    Hitbox hitbox;
    bool spacePressed;

public:
    Jugador(const sf::Texture &tex) : sprite(tex), hitbox(90.f) {
        sprite.setOrigin(75 / 2, 75 / 2);
        sprite.setTextureRect(sf::IntRect(0 * 75, 0 * 75, 75, 75));
        sprite.setPosition(320, 240);

        DataSingleton &data = DataSingleton::getInstance();
        data.playerPosition = sprite.getPosition();

        hitbox.setOrigin(sf::Vector2f(90.f, 90.f));
        hitbox.setPosition(sprite.getPosition());
        hitbox.setFillColor(sf::Color(0, 128, 255, 76));

    }

    void mover(sf::Keyboard::Key key) {
        DataSingleton &data = DataSingleton::getInstance();
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
            hitbox.detect();
            hitbox.executeAttack();
            data.soundPlayer.play("gunshot1");
            data.soundPlayer.setVolume(100.f);
            std::cout << "Disparo.\n";
            break;
        default:
            return;
        }
        data.playerPosition = sprite.getPosition();
        hitbox.setPosition(sprite.getPosition());
    }
    
    void updateInput(const sf::Time& deltaTime) {
        // Verificar si se presionó la tecla espacio
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !spacePressed) {
            //eliminarEnemigoMasCercano();
            spacePressed = true;
        }
        
        // Resetear el estado cuando se suelta la tecla
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            spacePressed = false;
        }
    }

    void updatePlayerAreas() {
        interactionArea.detect(DataSingleton::getInstance().getAreas());
    }

    void drawPlayer(sf::RenderWindow &window) {
        window.draw(interactionArea);
        window.draw(sprite);
    }
};

class Enemigo: public Entity {
private:
    sf::Sprite sprite;
    DetectedArea hurtbox;
public:
    Enemigo(const sf::Texture &tex) : sprite(tex), hurtbox(sf::RectangleShape(sf::Vector2f(75.f,100.f))) {
        sprite.setOrigin(75.f / 2.f, 75.f / 2.f);
        sprite.setTextureRect(sf::IntRect(4.6 * 75, 3.1 * 75, 75, 75));
        sprite.setPosition(50, 50);
        
        hurtbox.setOrigin(sf::Vector2f(37.5f, 50.f));
        hurtbox.setPosition(sprite.getPosition());
        hurtbox.setFillColor(sf::Color(255, 0, 0, 76));
    }

    void updateEnemy(float delta) {
        //calcular dirección de movimiento
        DataSingleton &data = DataSingleton::getInstance();
        sf::Vector2f targetPosition = data.playerPosition - sprite.getPosition();
        sf::Vector2f direction = normalize(targetPosition);

        // mover
        sprite.move(delta * ENEMY_SPEED * direction);
        hurtbox.setPosition(sprite.getPosition());
    }

    sf::Vector2f normalize(const sf::Vector2f& vector) {
        float length = std::sqrt(vector.x * vector.x + vector.y * vector.y);
        if (length != 0) {
            return vector / length;
        }
        return vector;  // Devuelve el vector original si la longitud es 0 (evita división por cero)
    }

    void drawEnemy(sf::RenderWindow &window) {
        hurtbox.draw(window);
        window.draw(sprite);
    }
};

int main() {
    // CONFIGURACIÓN DE VENTANA
    sf::RenderWindow window(sf::VideoMode(640, 480), "P0. Fundamentos de los Videojuegos. DCCIA");
    sf::Texture tex;
    if (!tex.loadFromFile("resources/sprites.png")) {
        std::cerr << "Error cargando la imagen sprites.png";
        return 1;
    }

    // CONFIGURACIÓN DE JUGADOR
    Jugador jugador(tex);
    Enemigo enemigo(tex);

    // CONFIGURACIÓN DE ÁREAS EN LA ESCENA
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

    // CONFIGURACIÓN SONIDO

    data.soundPlayer.loadSound("gunshot1", "./resources/gunshot1.ogg");
    data.soundPlayer.loadSound("gunshot2", "./resources/gunshot2.ogg");

    // CONFIGURACIÓN DE MÚSICA

    MusicPlayer musicPlayer;
    sf::Clock musicClock; // Reloj para medir el tiempo transcurrido

    // Cargar y reproducir la primera música con un fade in de 3 segundos
    musicPlayer.loadMusic("resources/Whispers_of_the_Abyss.ogg");
    musicPlayer.fadeIn(3.0f, 100.0f);
    musicClock.restart(); // Reiniciar el reloj cuando la música empieza

    bool transitionDone = false;
    
    sf::Clock clock;
    

    // BUCLE DE JUEGO
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed){
                jugador.mover(event.key.code);
            }
        }

        // if (clock.getElapsedTime().asSeconds() >= 10.0f) {
        //     data.soundPlayer.play("gunshot1");
        //     data.soundPlayer.play("gunshot2");
        //     clock.restart();
        //     std::cout << "Disparo dos veces.\n";
        // }

        // PRUEBA MÚSICA Y TRANSICIÓN

        musicPlayer.update();

        // Verificar si han pasado 10 segundos y aún no se ha hecho la transición
        if (musicClock.getElapsedTime().asSeconds() >= 10.0f && !transitionDone) {
            musicPlayer.transition("resources/The_Dark_Throne.ogg", 3.0f, 3.0f, 100.0f);
            musicClock.restart(); // Reiniciar el reloj para medir la duración de la nueva música
            transitionDone = true;
        }

        // Verificar si han pasado 10 segundos desde la transición y hacer fade out final
        if (transitionDone && musicClock.getElapsedTime().asSeconds() >= 10.0f) {
            musicPlayer.fadeOut(3.0f);
            transitionDone = false; // Reset para evitar múltiples fade outs
        }

        // UPDATE ------------

        float delta = clock.restart().asSeconds() * 60;
        sf::Time deltaTime = clock.restart();

        jugador.updateInput(deltaTime);
        jugador.updatePlayerAreas();
        enemigo.updateEnemy(delta);

        window.clear();
        for (DetectedArea *area : data.getAreas()) {
            if (area != nullptr)
                area->draw(window);
        }
        jugador.drawPlayer(window);
        enemigo.drawEnemy(window);
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