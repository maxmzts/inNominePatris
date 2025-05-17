#include "RoomState.h"
#include "EnemyManager.h"

RoomState::RoomState() : completed(false) {
}

RoomState::~RoomState() {
}

bool RoomState::isCompleted() const {
    return completed;
}

void RoomState::setCompleted(bool value) {
    completed = value;
}

bool RoomState::hasEnemies() const {
    return (EnemyManager::getInstance()->getAliveEnemyCount() != 0);
}