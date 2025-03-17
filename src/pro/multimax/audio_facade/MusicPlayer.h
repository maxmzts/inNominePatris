#pragma once

#include <SFML/Audio.hpp>
#include <string>
#include <memory>

class MusicPlayer {
public:
    enum class State { Stopped, Playing, Paused, FadingIn, FadingOut, Transitioning };
    enum class FadeType { None, In, Out };
    MusicPlayer();
    
    void loadMusic(const std::string& filename);
    void play();
    void pause();
    void stop();
    void fadeIn(float duration, float targetVolume);
    void fadeOut(float duration);
    void transition(const std::string& newFilename, float fadeOutDuration, float fadeInDuration, float targetVolume);
    void update();

    void setVolume(float volume);
    void setLoop(bool loop);
    void setPitch(float pitch);
    void setPosition(float x, float y, float z);
    
    bool isPlaying() const;
    bool isPaused() const;
    bool isStopped() const;
    State getState() const;

private:
    std::unique_ptr<sf::Music> music;
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
