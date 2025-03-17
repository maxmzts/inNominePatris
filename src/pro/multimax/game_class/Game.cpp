#include <iostream>
#include <vector>

#include "Game.h"

// FUNCIONES PRIVADAS

void Game::initVariables()
{
    this-> window = nullptr;

    //Inicializar lógica de juego
    this->points = 0;
    this->enemySpawnTimerMax = 60.f;
    this->enemySpawnTimer = this->enemySpawnTimerMax;
    this->maxEnemies = 10;
    this->mouseHeld = false;
}

void Game::initWindow()
{
    //Creamos una ventana
    this-> videomode.height = 480;
    this-> videomode.width = 600;
    // this->videomode.getDesktopMode()  for dinamic window
    this->window = new sf::RenderWindow(this->videomode, "Pruebas max", sf::Style::Titlebar | sf::Style::Close);
    this->window->setFramerateLimit(60);
}

void Game::initEnemies()
{
    this->enemy.setPosition(20.f,20.f);
    this->enemy.setSize(sf::Vector2f(100.f, 100.f));
    this->enemy.setScale(sf::Vector2f(0.5f, 0.5f));
    this->enemy.setFillColor(sf::Color::Red);
    this->enemy.setOutlineColor(sf::Color(180,0,0,255));
    this->enemy.setOutlineThickness(3.f);
}

// CONSTRUCTOR y DESTRUCTOR

Game::Game()
{
    this->initVariables();
    this->initEnemies();
    this->initWindow();
}

Game::~Game()
{
    delete this->window;
}

// FUNCIONES DE ACCESO
const bool Game::running() const
{
    return this->window->isOpen();
}


// FUNCIONES PÚBLICAS 

void Game::spawnEnemy()
{
    /*
        @return void
        
        Spawns enemies and sets their color and positions.
        - Asigna una posición aleatoria.
        - Asigna un color aleatorio.
        - Añade al enemigo al vector de enemigos.
    */
    
    this->enemy.setPosition(
        static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)),
        0.f
    );

    this->enemy.setFillColor(sf::Color::Green);

    //spawn enemy
    this->enemies.push_back(this->enemy);
}

void Game::pollEvents()
{
    //Bucle de obtención de eventos
    sf::Event event;
    while (this->window->pollEvent(this->event)){

        switch (this->event.type)
        {
        case sf::Event::Closed: //cierra la ventana al cerrar
            this->window->close();
            break;
        case sf::Event::KeyPressed: // cierra la ventana al pulsar Escape
            if (event.key.code == sf::Keyboard::Escape)
                this->window->close();
            break;
        default:
            break;
        }
    }
}

void Game::updateMousePositions()
{
    /*
        @ return void

        Actualiza la posición del mouse:
        - Obtiene la posición discreta en píxeles
        - Calcula la posición continua en float para cálculos
    */

    // posición discreta
    this->mousePosWindow = sf::Mouse::getPosition(*this->window);
    // posición continua
    this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void Game::updateEnemies()
{
    /*
        @ return void

        - Genera enemigos al terminar el contador si la cantidad de enemigos 
          en pantalla es menor al máximo.
        - Mueve los enemigos abajo.
    */

    // Actualizar timer para generar enemigos
    if(this->enemies.size() < this->maxEnemies)
    {
        if(this->enemySpawnTimer >= this->enemySpawnTimerMax)
        {
            // Crea el enemigo cuando el contador llega al máximo
            this->spawnEnemy();
            this->enemySpawnTimer = 0.f;
        }
        else
            this->enemySpawnTimer += 1.f;
    }

    // Mover enemigos hacia abajo y eliminarlos
    for (int i = 0; i < this->enemies.size(); i++)
    {
        bool deleted = false;

        this->enemies[i].move(0.f, 2.f);

        // eliminar enemigos si salen de la pantalla
        if(this->enemies[i].getPosition().y > this->window->getSize().y)
            this->enemies.erase(this->enemies.begin() + i);
    }

    //si se ha clicado la pantalla se comprueba que se ha clicado sobre un enemigo
    //ponerlo fuera del loop anterior evita poder eliminar enemigos manteniendo el clic
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        if(this->mouseHeld == false)
        {
            this->mouseHeld = true;
            bool deleted = false;
            for (size_t i = 0; i < this->enemies.size() && deleted == false; i++)
            {
                if(this->enemies[i].getGlobalBounds().contains(this->mousePosView))
                {
                    deleted = true;
                    this->enemies.erase(this->enemies.begin() + i);

                    // conseguir puntos
                    this->points += 1.f;
                }
            }
        }
    }
    else
    {
        this->mouseHeld = false;
    }
}

void Game::renderEnemies()
{
    for (auto &e : this->enemies)
    {
        this->window->draw(e);
    }
}

void Game::update()
{
    this->pollEvents();

    this->updateMousePositions();

    this->updateEnemies();

    //Posicion relativa a la pantalla del usuario
    // std::cout << "Mouse pos: " << sf::Mouse::getPosition().x << " , " << sf::Mouse::getPosition().y << "\n";
    //Posicion del mouse relativa a la ventana del juego (puede tener valores negativos)
    //std::cout << "Mouse pos: " 
    //    << sf::Mouse::getPosition(*this->window).x << " , " 
    //    << sf::Mouse::getPosition(*this->window).y << "\n";
}

void Game::render()
{
    /*
        @return void

        - clear old frame
        - render objects
        - display frame in window

        Renders the game objects
    */

    this->window->clear();

    //draw game objects
    this->renderEnemies();

    //draw objects
    this->window->display();
}