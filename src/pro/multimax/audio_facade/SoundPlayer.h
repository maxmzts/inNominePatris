#pragma once

#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include <memory>

class SoundPlayer {
public:
    SoundPlayer();

    // Cargar un sonido en memoria
    void loadSound(const std::string& name, const std::string& filename);
    
    // Reproducir un sonido
    void play(const std::string& name);
    
    // Pausar un sonido
    void pause(const std::string& name);
    
    // Parar un sonido
    void stop(const std::string& name);
    
    // Pausar todos los sonidos activos
    void pauseAll();
    
    // Detener todos los sonidos activos
    void stopAll();
    
    // Ajustar el volumen general de los sonidos
    void setVolume(float volume);
    
    // Ajustar el pitch de un sonido
    void setPitch(const std::string& name, float pitch);
    
    // Asignar la posición de un sonido en el espacio 3D
    void setPosition(const std::string& name, float x, float y, float z);

    void removeStoppedSounds();

private:
    std::map<std::string, std::unique_ptr<sf::SoundBuffer>> soundBuffers;
    std::vector<sf::Sound> sounds;
    float volume;
};
