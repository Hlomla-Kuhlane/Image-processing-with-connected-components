#include "PGMimageProcessor.h"
#include <queue>
#include <climits> 
#include <algorithm>
#include <iostream>

// Constructor: Reads the PGM image file when creating a new processor
PGMimageProcessor::PGMimageProcessor(const std::string& filename) 
    : currentThreshold(0) {
    image.reset(new PGMimage());
    image->read(filename);  // Read the image file
    width = image->getWidth();  // Store image width
    height = image->getHeight();  // Store image height
}

// Destructor: Clean up resources
PGMimageProcessor::~PGMimageProcessor() {}

// Move constructor
PGMimageProcessor::PGMimageProcessor(PGMimageProcessor&& other) noexcept
    : components(std::move(other.components)),
      image(std::move(other.image)),
      width(other.width),
      height(other.height),
      currentThreshold(other.currentThreshold) {
    // Reset the source object
    other.width = 0;
    other.height = 0;
    other.currentThreshold = 0;
}

// Move assignment operator
PGMimageProcessor& PGMimageProcessor::operator=(PGMimageProcessor&& other) noexcept {
    if (this != &other) {
        components = std::move(other.components);
        image = std::move(other.image);
        width = other.width;
        height = other.height;
        currentThreshold = other.currentThreshold;
        
        // Reset the source object
        other.width = 0;
        other.height = 0;
        other.currentThreshold = 0;
    }
    return *this;
}

// Breadth-First Search to find connected components (groups of white pixels)
void PGMimageProcessor::bfs(int startX, int startY, std::unique_ptr<ConnectedComponent>& component) {
    std::queue<std::pair<int,int>> pixelQueue;  // Queue for BFS
    pixelQueue.push(std::make_pair(startX, startY));  // Start from given pixel
    image->setPixel(startX, startY, 0);  // Mark as visited by setting to black

    // Directions to check: up, down, left, right
    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    while (!pixelQueue.empty()) {
        std::pair<int,int> current = pixelQueue.front();
        pixelQueue.pop();
        component->addPixel(current.first, current.second);  // Add to component

        // Check all 4 neighboring pixels
        for (int i = 0; i < 4; i++) {
            int newX = current.first + directions[i][0];
            int newY = current.second + directions[i][1];

            // If neighbor is within image bounds and is white
            if (newX >= 0 && newX < width && newY >= 0 && newY < height) {
                if (image->getPixel(newX, newY) == 255) {
                    pixelQueue.push(std::make_pair(newX, newY));
                    image->setPixel(newX, newY, 0);  // Mark as visited
                }
            }
        }
    }
}

// Convert image to black and white based on threshold value
void PGMimageProcessor::thresholdImage(unsigned char thresholdValue) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            unsigned char currentPixel = image->getPixel(x, y);
            // Set pixel to white if >= threshold, black otherwise
            if (currentPixel >= thresholdValue) {
                image->setPixel(x, y, 255);
            } else {
                image->setPixel(x, y, 0);
            }
        }
    }
}

// Find all connected components in the image
int PGMimageProcessor::extractComponents(unsigned char threshold, int minValidSize) {
    components.clear();  // Clear previous components
    thresholdImage(threshold);  // First apply threshold

    // Scan the entire image for white pixels
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (image->getPixel(x, y) == 255) {  // Found a white pixel
                std::unique_ptr<ConnectedComponent> newComponent(new ConnectedComponent(components.size()));
                bfs(x, y, newComponent);  // Find all connected pixels

                // Only keep components larger than minValidSize
                if (newComponent->getPixelCount() >= minValidSize) {
                    components.push_back(std::move(newComponent));
                }
            }
        }
    }
    return static_cast<int>(components.size());  // Return number of components found
}

// Remove components that are too small or too large
int PGMimageProcessor::filterComponentsBySize(int minSize, int maxSize) {
    auto it = components.begin();
    while (it != components.end()) {
        int currentSize = (*it)->getPixelCount();
        // Remove if size is outside the allowed range
        if (currentSize < minSize || currentSize > maxSize) {
            it = components.erase(it);
        } else {
            ++it;
        }
    }
    return static_cast<int>(components.size());  // Return remaining count
}

// Save the components to a new image file
bool PGMimageProcessor::writeComponents(const std::string& outFileName) {
    if (components.empty()) {  // Nothing to write
        return false;
    }

    unsigned char* outputImage = new unsigned char[width * height]();

    try {
        std::fill(outputImage, outputImage + width * height, 0);  // Start with black image

        // Draw all components as white pixels
        for (size_t i = 0; i < components.size(); i++) {
            const auto& pixels = components[i]->getPixels();
            for (const auto& pixel : pixels) {
                outputImage[pixel.second * width + pixel.first] = 255;
            }
        }

        // Create and save the output image
        PGMimage resultImage;
        resultImage.setImageData(outputImage, width, height);
        resultImage.write(outFileName);
        
        delete[] outputImage;  // Clean up
        return true;
    } catch (const std::exception& e) {
        delete[] outputImage;  // Clean up if error occurs
        return false;
    }
}

// Count components larger than current threshold
int PGMimageProcessor::getComponentCount() const {
    int count = 0;
    for (const auto& comp : components) {
        if (comp->getPixelCount() > currentThreshold) {
            count++;
        }
    }
    return count;
}

// Set the minimum size threshold for components
void PGMimageProcessor::setCurrentThreshold(int threshold) {
    currentThreshold = threshold;
}

// Find the largest component size above threshold
int PGMimageProcessor::getLargestSize() const {
    int maxSize = 0;
    for (const auto& comp : components) {
        int size = comp->getPixelCount();
        if (size > currentThreshold && size > maxSize) {
            maxSize = size;
        }
    }
    return maxSize;
}

// Find the smallest component size above threshold
int PGMimageProcessor::getSmallestSize() const {
    int minSize = INT_MAX;
    for (const auto& comp : components) {
        int size = comp->getPixelCount();
        if (size > currentThreshold && size < minSize) {
            minSize = size;
        }
    }
    return (minSize == INT_MAX) ? 0 : minSize;
}

// Print information about components larger than threshold
void PGMimageProcessor::printComponentsAboveThreshold() const {
    for (const auto& comp : components) {
        if (comp->getPixelCount() > currentThreshold) {
            printComponentData(*comp);
        }
    }
}

// Print details about a single component
void PGMimageProcessor::printComponentData(const ConnectedComponent& theComponent) const {
    std::cout << "Component " << theComponent.getId() << ": " 
              << theComponent.getPixelCount() << " pixels\n";
}