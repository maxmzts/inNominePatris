#pragma once
#include "Interaction.h"
#include <memory>

class InteractionFactory {
public:
    static std::shared_ptr<Interaction> createInteraction(int tileId);
};