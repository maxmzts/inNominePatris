#include "RoomManager.h"

// Inicialización del puntero estático
RoomManager* RoomManager::instance = nullptr;

RoomManager::RoomManager() : currentState(nullptr) {
}

RoomManager* RoomManager::getInstance() {
    if (!instance) {
        instance = new RoomManager();
    }
    return instance;
}

RoomManager::~RoomManager() {
    states.clear();
    currentState = nullptr;
}

void RoomManager::registerState(const std::string& stateId, std::shared_ptr<RoomState> state) {
    states[stateId] = state;
}

void RoomManager::changeState(const std::string& stateId) {
    auto it = states.find(stateId);
    if (it != states.end()) {
        if (currentState) {
            currentState->exit();
        }
        
        currentState = it->second;
        currentState->enter();
    }
}

void RoomManager::update() {
    if (currentState) {
        currentState->update();
    }
}

std::shared_ptr<RoomState> RoomManager::getCurrentState() const {
    return currentState;
}