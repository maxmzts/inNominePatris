#include "MusicManager.h"
#include <iostream>

MusicManager& MusicManager::getInstance() {
    static MusicManager instance;
    return instance;
}

void MusicManager::addTrack(
    const std::string& texturePath,
    float volume, 
    bool loop,
    float pitch
) {
    // tracks.emplace_back(std::make_unique<MusicTrack>(
    //     texturePath, volume, pitch
    // ));
    auto newTrack = std::make_unique<MusicTrack>(texturePath, volume, pitch);
    if (newTrack->isValid()) {
        tracks.emplace_back(std::move(newTrack));
    } else {
        std::cerr << "Error al cargar pista en addTrack: " << texturePath << std::endl;
    }
}

void MusicManager::transitionTo(
    const std::string& soundPath,
    float volume,
    float pitch,
    float transitionDuration
) {
    if (transitionDuration <= 0.0f) {
        // Si no hay transición, simplemente detén todas las pistas y añade la nueva
        clear();
        addTrack(soundPath, volume, true, pitch);
        return;
    }

    // Verificamos si ya existe la pista de destino
    bool destTrackExists = false;
    
    // Primero revisamos en activeTransitions
    for (auto& transition : activeTransitions) {
        if (transition.track && transition.track->isValid() && transition.track->getFilename() == soundPath) {
            // La pista ya está en transición, actualizamos sus parámetros
            transition.targetVolume = volume;
            transition.transitionDuration = transitionDuration;
            transition.transitionTime = 0.0f;
            transition.fadeOut = false;
            destTrackExists = true;
            // Asegurémonos de que esté reproduciéndose
            if (!transition.track->isPlaying()) {
                transition.track->play();
            }
        }
    }
    
    // Luego en las pistas regulares
    for (auto it = tracks.begin(); it != tracks.end(); ) {
        if ((*it)->getFilename() == soundPath) {
            // Si es la pista destino, la movemos a transiciones
            destTrackExists = true;
            
            TrackTransition transition;
            transition.track = std::move(*it);
            if (transition.track && transition.track->isValid()) {
                transition.initialVolume = transition.track->getVolume();
            }
            transition.targetVolume = volume;
            transition.transitionDuration = transitionDuration;
            transition.transitionTime = 0.0f;
            transition.fadeOut = false;
            
            activeTransitions.push_back(std::move(transition));
            
            // Remover de la lista regular
            it = tracks.erase(it);
        } else {
            // Para todas las demás pistas, configúralas para desvanecerse
            TrackTransition transition;
            transition.track = std::move(*it);
            if (transition.track && transition.track->isValid()) {
                transition.initialVolume = transition.track->getVolume();
            }
            transition.targetVolume = 0.0f;
            transition.transitionDuration = transitionDuration;
            transition.transitionTime = 0.0f;
            transition.fadeOut = true;
            
            activeTransitions.push_back(std::move(transition));
            
            // Avanzar al siguiente
            it = tracks.erase(it);
        }
    }
    
    // Si la pista de destino aún no existe, añádela
    if (!destTrackExists) {
        // Crear la nueva pista con volumen inicial 0
        auto newTrack = std::make_unique<MusicTrack>(soundPath, 0.0f, true, pitch);
        
        // IMPORTANTE: Verificar si se cargó correctamente
        if (newTrack->isValid()) {
            newTrack->play();
            
            // Configura la transición para la nueva pista
            TrackTransition transition;
            transition.track = std::move(newTrack);
            transition.initialVolume = 0.0f;
            transition.targetVolume = volume;
            transition.transitionDuration = transitionDuration;
            transition.transitionTime = 0.0f;
            transition.fadeOut = false;
            
            activeTransitions.push_back(std::move(transition));
        } else {
            std::cerr << "Error al cargar el archivo para transición: " << soundPath << std::endl;
        }
    }
}

// Depuración en update para ver qué está pasando
void MusicManager::update(float deltaTime) {
    // DEBUG
    // std::cout << "Estado MusicManager - Pistas regulares: " << tracks.size() 
    //           << ", Transiciones: " << activeTransitions.size() << std::endl;
    
    // Actualizar las transiciones activas
    for (auto it = activeTransitions.begin(); it != activeTransitions.end(); ) {
        auto& transition = *it;
        
        if (!transition.track) {
            //std::cerr << "Error: Puntero nulo detectado en transición" << std::endl;
            it = activeTransitions.erase(it);
            continue;
        }
        
        // Incrementar el tiempo de transición
        transition.transitionTime += deltaTime;
        float t = std::min(transition.transitionTime / transition.transitionDuration, 1.0f);
        
        // Calcular el volumen actual basado en la interpolación lineal
        float currentVolume;
        if (transition.fadeOut) {
            // Si estamos desvaneciendo, disminuir el volumen
            currentVolume = transition.initialVolume * (1.0f - t);
        } else {
            // Si estamos apareciendo, aumentar el volumen
            currentVolume = transition.initialVolume + (transition.targetVolume - transition.initialVolume) * t;
        }
        
        // Asegurarnos de que el volumen está en un rango válido
        currentVolume = std::max(0.0f, std::min(100.0f, currentVolume));
        
        // Aplicar el volumen a la pista si existe 
        if(transition.track && transition.track->isValid()){
            try {
                transition.track->setVolume(currentVolume);
            } catch (...) {
                std::cerr << "Excepción al establecer volumen en MusicTrack." << std::endl;
            }
        }
        
        // Verificar que la pista esté reproduciéndose
        if (transition.track->isValid() && !transition.track->isPlaying() && !transition.fadeOut) {
            // std::cout << "Reiniciando reproducción de pista en transición" << std::endl;
            transition.track->play();
        }
        
        // Comprobar si la transición ha terminado
        if (transition.track->isValid() && transition.transitionTime >= transition.transitionDuration) {
            if (transition.fadeOut ) {
                transition.track->stop();
                it = activeTransitions.erase(it);
            } else {
                // GUARDA TEMPORALMENTE EL PUNTERO ANTES DEL MOVE
                auto track = std::move(transition.track);
                track->setVolume(transition.targetVolume);
                tracks.push_back(std::move(track));
                it = activeTransitions.erase(it);
            }
        } else {
            ++it;
        }
    }
    
    // Eliminar las pistas detenidas como en la implementación original
    size_t beforeSize = tracks.size();
    tracks.erase(
        std::remove_if(tracks.begin(), tracks.end(),
            [](const std::unique_ptr<MusicTrack>& e) {
                return !e || e->isStopped();
            }),
        tracks.end()
    );
    if (beforeSize != tracks.size()) {
        // std::cout << "Se eliminaron " << (beforeSize - tracks.size()) << " pistas detenidas" << std::endl;
    }
}

void MusicManager::clear() {
    for (auto& track : tracks) {
        if (track && track->isValid()) track->stop();
    }
    for (auto& transition : activeTransitions) {
        if (transition.track && transition.track->isValid()) transition.track->stop();
    }
}



