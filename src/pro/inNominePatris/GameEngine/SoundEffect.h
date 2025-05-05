#pragma once

#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include <memory>

class SoundEffect {
public:
    SoundEffect(const std::string& filename, float volume = 100.f, float pitch = 1.f);

    // Cargar un sonido en memoria
    void loadSound(const std::string& filename);
    
    // Reproducir un sonido
    void play();
    
    // Pausar un sonido
    void pause();
    
    // Parar un sonido
    void stop();
    
    // Ajustar el volumen general de los sonidos
    void setVolume(float volume);
    
    // Ajustar el pitch de un sonido
    void setPitch(float pitch);

    bool isStopped();

private:
    sf::SoundBuffer soundBuffer;
    sf::Sound sound;
    float volume;
    enum class Status { Stopped , Paused , Playing };
    Status currentStatus;
};
