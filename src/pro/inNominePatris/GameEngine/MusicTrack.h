#pragma once

#include <SFML/Audio.hpp>
#include <string>
#include <memory>

class MusicTrack {
public:
    enum class State { Stopped, Playing, Paused, FadingIn, FadingOut, Transitioning };
    enum class FadeType { None, In, Out };
    MusicTrack(const std::string& filename, float volume = 100.f, bool loop = true, float pitch = 1.f);
    
    void loadMusic(const std::string& filename);
    void play();
    void pause();
    void stop();

    void setVolume(float volume);
    void setLoop(bool loop);
    void setPitch(float pitch);
    
    bool isPlaying() const { return music.getStatus() == sf::Music::Playing; }
    bool isPaused() const;
    bool isStopped() const;
    bool isTransitioning() const;
    State getState() const;

private:
    sf::Music music;
    State currentState;
    sf::Clock fadeClock;
    float fadeDuration;
    float fadeTargetVolume;
    FadeType fadeType;

    std::string transitionFile;
    float transitionFadeInDuration;
    float transitionFadeOutDuration;
    float transitionTargetVolume;

};
