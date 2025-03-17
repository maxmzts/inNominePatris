#include "SoundPlayer.h"
#include <stdexcept>
#include <algorithm>

SoundPlayer::SoundPlayer() : volume(100.0f) {}

void SoundPlayer::loadSound(const std::string& name, const std::string& filename) {
    auto buffer = std::make_unique<sf::SoundBuffer>();
    if (!buffer->loadFromFile(filename)) {
        throw std::runtime_error("Error cargando el sonido: " + filename);
    }
    soundBuffers[name] = std::move(buffer);
}

void SoundPlayer::play(const std::string& name) {
    if (soundBuffers.find(name) == soundBuffers.end()) {
        throw std::runtime_error("Sonido no encontrado: " + name);
    }
    
    sounds.emplace_back(); // Agrega un nuevo sonido al vector
    sf::Sound& sound = sounds.back(); // Referencia al último sonido agregado
    sound.setBuffer(*soundBuffers[name]);
    sound.setVolume(volume);
    sound.play();
    
    // removeStoppedSounds() SE ELIMINA AQUÍ para evitar problemas de eliminación prematura
}

void SoundPlayer::pause(const std::string& name) {
    for (auto& sound : sounds) {
        if (sound.getBuffer() == soundBuffers[name].get() && sound.getStatus() == sf::Sound::Playing) {
            sound.pause();
        }
    }
}

void SoundPlayer::stop(const std::string& name) {
    for (auto& sound : sounds) {
        if (sound.getBuffer() == soundBuffers[name].get() && sound.getStatus() != sf::Sound::Stopped) {
            sound.stop();
        }
    }
}

void SoundPlayer::setVolume(float newVolume) {
    volume = newVolume;
    for (auto& sound : sounds) {
        sound.setVolume(volume);
    }
}

void SoundPlayer::setPitch(const std::string& name, float pitch) {
    for (auto& sound : sounds) {
        if (sound.getBuffer() == soundBuffers[name].get()) {
            sound.setPitch(pitch);
        }
    }
}

void SoundPlayer::setPosition(const std::string& name, float x, float y, float z) {
    for (auto& sound : sounds) {
        if (sound.getBuffer() == soundBuffers[name].get()) {
            sound.setPosition(x, y, z);
        }
    }
}

void SoundPlayer::pauseAll() {
    for (auto& sound : sounds) {
        if (sound.getStatus() == sf::Sound::Playing) {
            sound.pause();
        }
    }
}

void SoundPlayer::stopAll() {
    for (auto& sound : sounds) {
        if (sound.getStatus() != sf::Sound::Stopped) {
            sound.stop();
        }
    }
    removeStoppedSounds(); // Aquí SÍ llamamos a la limpieza porque queremos forzar el borrado
}

void SoundPlayer::removeStoppedSounds() {
    sounds.erase(std::remove_if(sounds.begin(), sounds.end(),
                                [](const sf::Sound& s) { return s.getStatus() == sf::Sound::Stopped; }),
                 sounds.end());
}