#include "MusicPlayer.h"
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

// Constructor: inicializa el estado y las variables de control del fade.
MusicPlayer::MusicPlayer() : currentState(State::Stopped), music(std::make_unique<sf::Music>()), fadeClock(), fadeDuration(0.0f), fadeTargetVolume(100.0f), fadeType(FadeType::None) {}

// Carga un archivo de música en el reproductor.
void MusicPlayer::loadMusic(const std::string& filename) {
    if (!music->openFromFile(filename)) {
        throw std::runtime_error("Error cargando la música: " + filename);
    }
}

// Inicia la reproducción de la música.
void MusicPlayer::play() {
    music->play();
    currentState = State::Playing;
}

// Pausa la música actual.
void MusicPlayer::pause() {
    music->pause();
    currentState = State::Paused;
}

// Detiene la música actual.
void MusicPlayer::stop() {
    music->stop();
    currentState = State::Stopped;
}

// Aplica un fade-in a la música, aumentando gradualmente el volumen.
void MusicPlayer::fadeIn(float duration, float targetVolume) {
    if (currentState == State::Playing) return; // Evita aplicar fade-in si ya está en reproducción.
    
    music->setVolume(0); // Comienza desde volumen 0.
    play();
    fadeClock.restart(); // Reinicia el reloj para medir el tiempo del fade.
    fadeDuration = duration;
    fadeTargetVolume = targetVolume;
    fadeType = FadeType::In;
    currentState = State::FadingIn;
}

// Aplica un fade-out a la música, reduciendo gradualmente el volumen hasta detenerla.
void MusicPlayer::fadeOut(float duration) {
    if (currentState != State::Playing) return; // Solo aplica fade-out si está en reproducción.
    
    fadeClock.restart();
    fadeDuration = duration;
    fadeTargetVolume = 0.0f; // El volumen objetivo es 0.
    fadeType = FadeType::Out;
    currentState = State::FadingOut;
}

// Transición entre dos pistas con fade-out en la actual y fade-in en la nueva.
void MusicPlayer::transition(const std::string& newFilename, float fadeOutDuration, float fadeInDuration, float targetVolume) {
    if (currentState != State::Playing) return; // Solo permite la transición si hay música sonando.
    
    transitionFile = newFilename; // Guarda el nombre del archivo a cargar.
    transitionFadeOutDuration = fadeOutDuration;
    transitionFadeInDuration = fadeInDuration;
    transitionTargetVolume = targetVolume;
    fadeOut(fadeOutDuration); // Inicia el fade-out de la pista actual.
}

// Método que debe llamarse regularmente en el bucle principal para actualizar los fades.
void MusicPlayer::update() {
    if (fadeType == FadeType::None) return; // No hace nada si no hay un fade en curso.
    
    float elapsed = fadeClock.getElapsedTime().asSeconds(); // Obtiene el tiempo transcurrido.
    if (fadeType == FadeType::In) {
        // Calcula el volumen actual en función del tiempo transcurrido.
        float volume = (elapsed / fadeDuration) * fadeTargetVolume;
        music->setVolume(volume > fadeTargetVolume ? fadeTargetVolume : volume);
        if (elapsed >= fadeDuration) {
            fadeType = FadeType::None;
            currentState = State::Playing; // El fade-in ha terminado.
        }
    } else if (fadeType == FadeType::Out) {
        float startVolume = music->getVolume();
        float volume = startVolume * (1.0f - (elapsed / fadeDuration)); // Reduce el volumen gradualmente.
        music->setVolume(volume < 0 ? 0 : volume);
        if (elapsed >= fadeDuration) {
            stop(); // Detiene la música al finalizar el fade-out.
            fadeType = FadeType::None;
            if (!transitionFile.empty()) {
                // Si hay un archivo en cola para transición, lo carga y reproduce con fade-in.
                auto newMusic = std::make_unique<sf::Music>();
                if (newMusic->openFromFile(transitionFile)) {
                    music = std::move(newMusic);
                    fadeIn(transitionFadeInDuration, transitionTargetVolume);
                }
                transitionFile.clear(); // Borra el nombre del archivo una vez realizado el cambio.
            }
        }
    }
}

// Ajusta el volumen de la música.
void MusicPlayer::setVolume(float volume) {
    music->setVolume(volume);
}

// Activa o desactiva el bucle en la reproducción.
void MusicPlayer::setLoop(bool loop) {
    music->setLoop(loop);
}

// Modifica el tono de la música.
void MusicPlayer::setPitch(float pitch) {
    music->setPitch(pitch);
}

// Establece la posición del sonido en el espacio 3D.
void MusicPlayer::setPosition(float x, float y, float z) {
    music->setPosition(x, y, z);
}

// Comprueba si la música está reproduciéndose.
bool MusicPlayer::isPlaying() const {
    return music->getStatus() == sf::Music::Playing;
}

// Comprueba si la música está pausada.
bool MusicPlayer::isPaused() const {
    return music->getStatus() == sf::Music::Paused;
}

// Comprueba si la música está detenida.
bool MusicPlayer::isStopped() const {
    return music->getStatus() == sf::Music::Stopped;
}

// Devuelve el estado actual del reproductor.
MusicPlayer::State MusicPlayer::getState() const {
    return currentState;
}