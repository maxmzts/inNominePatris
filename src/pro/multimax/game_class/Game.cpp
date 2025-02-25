#include "Game.h"

// FUNCIONES PRIVADAS

void Game::initVariables()
{
    this-> window = nullptr;
}
void Game::initWindow()
{
    //Creamos una ventana
    this-> videomode.height = 480;
    this-> videomode.width = 600;
    // this->videomode.getDesktopMode()  for dinamic window
    this->window = new sf::RenderWindow(this->videomode, "Pruebas max", sf::Style::Titlebar | sf::Style::Close);

}

// CONSTRUCTOR y DESTRUCTOR

Game::Game()
{
    this->initVariables();
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

void Game::update()
{
    this->pollEvents();
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

    this->window->clear(sf::Color::Red);

    //draw objects
    this->window->display();
}