#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <ctime>

/*
    Clase para encapsular el motor de juego.
    Funciona además como sigleton para gestionar la información del juego.
*/

class Game
{
private:
    // variables

    //window
    sf::RenderWindow* window; 
    sf::VideoMode videomode;
    sf::Event event;

    //mouse positions
    sf::Vector2i mousePosWindow;
    sf::Vector2f mousePosView;

    //Game logic
    unsigned points;
    float enemySpawnTimer;
    float enemySpawnTimerMax;
    int maxEnemies;
    bool mouseHeld;

    //Game objects
    std::vector<sf::RectangleShape> enemies;
    sf::RectangleShape enemy;

    //Inicializadores
    void initVariables();
    void initWindow();
    void initEnemies();
    

public:
    //constructors and destructors
    Game();
    virtual ~Game();

    //funciones de acceso
    const bool running() const;

    //functions
    void spawnEnemy();

    void pollEvents();
    void updateMousePositions();
    void updateEnemies();
    void renderEnemies();
    void update();
    void render();
};

