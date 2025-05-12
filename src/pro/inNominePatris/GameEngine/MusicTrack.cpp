#include "MusicTrack.h"
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

// Constructor: inicializa el estado y las variables de control del fade.
MusicTrack::MusicTrack(const std::string& filename, float volume, bool loop, float pitch) {
    loadMusic(filename);
    setVolume(volume);
    setLoop(loop);
    setPitch(pitch);
    this->filename = filename;
}

// Carga un archivo de música en el reproductor.
void MusicTrack::loadMusic(const std::string& filename) {
    if (!music.openFromFile(filename)) {
        throw std::runtime_error("Error cargando la música: " + filename);
    }
}

// Inicia la reproducción de la música.
void MusicTrack::play() {
    music.play();
}

// Pausa la música actual.
void MusicTrack::pause() {
    music.pause();
}

// Detiene la música actual.
void MusicTrack::stop() {
    music.stop();
}

// Ajusta el volumen de la música.
void MusicTrack::setVolume(float volume) {
    music.setVolume(volume);
}

// Activa o desactiva el bucle en la reproducción.
void MusicTrack::setLoop(bool loop) {
    music.setLoop(loop);
}

// Modifica el tono de la música.
void MusicTrack::setPitch(float pitch) {
    music.setPitch(pitch);
}

// Comprueba si la música está pausada.
bool MusicTrack::isPaused() const {
    return music.getStatus() == sf::Music::Paused;
}

// Comprueba si la música está detenida.
bool MusicTrack::isStopped() const {
    return music.getStatus() == sf::Music::Stopped;
}