#pragma once

#include <vector>
#include <algorithm>
#include <cmath>

#include "DetectedArea.h"

// Clase abstracta que representa un área que detecta instancias de DetectedArea
class DetectingArea : public sf::CircleShape {
protected:
    std::vector<DetectedArea*> detectedObjects; // Vector ordenado de objetos detectados

public:
    DetectingArea(float radius);
    virtual ~DetectingArea();

    // Método para detectar y almacenar objetos DetectedArea
    void detect(const std::vector<DetectedArea*>& areas);

    // Método para ordenar las áreas detectadas en función de la distancia
    void sortDetectedObjects();
    
    // Método abstracto para definir la lógica de detección
    virtual bool isDetected(const DetectedArea& area) const = 0;

    // Obtener la distancia de un área detectada al centro de esta área
    float getDistance(const DetectedArea& area) const;

    // Obtener las áreas detectadas
    const std::vector<DetectedArea*>& getDetectedArea() const;
    
    // Obtener las áreas detectadas
    const DetectedArea* getClosestArea() const;

    // Interactuar con el primer Area de la lista
    virtual void interact() = 0;

};