#include "DetectingArea.h"

DetectingArea::DetectingArea(float radius) : sf::CircleShape(radius) {}

DetectingArea::~DetectingArea(){}

void DetectingArea::detect(const std::vector<DetectedArea*>& areas) {
    detectedObjects.clear();
    
    for (auto* area : areas) {
        if (isDetected(*area)) {
            detectedObjects.push_back(area);
        }
    }
    
    // Ordenar el vector según algún criterio (por ejemplo, distancia al centro del área de detección)
    sortDetectedObjects();
}

void DetectingArea::sortDetectedObjects() {
    std::sort(detectedObjects.begin(), detectedObjects.end(), [this](DetectedArea* a, DetectedArea* b) {
        return getDistance(*a) < getDistance(*b);
    });
}

float DetectingArea::getDistance(const DetectedArea& area) const {
    sf::Vector2f diff = area.getPosition() - getPosition();
    return std::sqrt(diff.x * diff.x + diff.y * diff.y);
}

const std::vector<DetectedArea*>& DetectingArea::getDetectedArea() const {
    return detectedObjects;
}

const DetectedArea* DetectingArea::getClosestArea() const {
    return detectedObjects[0];
}