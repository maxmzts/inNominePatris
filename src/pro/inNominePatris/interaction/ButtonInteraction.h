#pragma once
#include "Interaction.h"

class ButtonInteraction : public Interaction {
public:
    ButtonInteraction(int id, const std::string& name, int centerX, int centerY);
    virtual ~ButtonInteraction() = default;
    
    void execute(Character& character, TileMap& tilemap) override;
    
private:
    int m_centerX;
    int m_centerY;
};