#include "SoundEffect.h"
#include <stdexcept>
#include <algorithm>

SoundEffect::SoundEffect(const std::string& filename, float volume, float pitch) {
    loadSound(filename);
    setVolume(volume);
    setPitch(pitch);
    play();
}

void SoundEffect::loadSound(const std::string& filename) {
    if (!soundBuffer.loadFromFile(filename)) {
        throw std::runtime_error("Error cargando el sonido: " + filename);
    }
}

void SoundEffect::play() {
    sound.setBuffer(soundBuffer);
    sound.setVolume(volume);
    sound.play();
    currentStatus = Status::Playing;
}

void SoundEffect::pause() {
    sound.pause();
    currentStatus = Status::Paused;
    
}

void SoundEffect::stop() {
    sound.stop();
    currentStatus = Status::Stopped;
}

void SoundEffect::setVolume(float newVolume) {
    volume = newVolume;
    sound.setVolume(volume);
}


void SoundEffect::setPitch(float pitch) {
    sound.setPitch(pitch);
}

bool SoundEffect::isStopped(){
    if(currentStatus == Status::Stopped)
        return true;
    else 
        return false;
}
