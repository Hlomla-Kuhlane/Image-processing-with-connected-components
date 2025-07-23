#include "ConnectedComponent.h"


int ConnectedComponent::minPixelThreshold = 0;  // Default minimum size for a component to count

// Constructor
ConnectedComponent::ConnectedComponent(int id) 
    : id(id),         // Store the component ID
      pixelCount(0)   // Start with 0 pixels
{}

// Add a pixel to this component
void ConnectedComponent::addPixel(int x, int y) {
    pixels.push_back({x, y});  // Store pixel coordinates
    pixelCount++;              
}

// Get this component's ID number
int ConnectedComponent::getId() const {
    return id;
}

// Get how many pixels are in this component
int ConnectedComponent::getPixelCount() const {
    if (pixelCount >= minPixelThreshold) {
        return pixelCount;  
    }
    return 0;  
}

// Get all pixel coordinates in this component
const std::vector<std::pair<int, int>>& ConnectedComponent::getPixels() const {
    return pixels;
}

// Change the minimum size requirement for all components
void ConnectedComponent::setMinPixelThreshold(int threshold) {
    minPixelThreshold = threshold;
}