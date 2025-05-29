#pragma once

#include <SFML/Audio.hpp>
#include <string>
#include <memory>

class MusicTrack {
public:
    MusicTrack(const std::string& filename, float volume = 100.f, bool loop = true, float pitch = 1.f);
    void loadMusic(const std::string& filename);
    void play();
    void pause();
    void stop();
    void setVolume(float volume);
    float getVolume() const { return music.getVolume(); }
    void setLoop(bool loop);
    void setPitch(float pitch);
    bool isPlaying() const { return music.getStatus() == sf::Music::Playing; }
    bool isPaused() const;
    bool isStopped() const;
    bool isValid() const;
    const std::string& getFilename() const { return filename; }
    
private:
    sf::Music music;
    bool loadedSuccessfully = false;
    std::string filename;
};
