#ifndef CONNECTED_COMPONENT_H
#define CONNECTED_COMPONENT_H

#include <vector>
#include <utility> 

class ConnectedComponent {
private:
    int id;
    int pixelCount;
    std::vector<std::pair<int, int>> pixels;
    static int minPixelThreshold; // Minimum pixels to consider a component "large"

public:
    explicit ConnectedComponent(int id);
    void addPixel(int x, int y);
    int getId() const;
    int getPixelCount() const;
    const std::vector<std::pair<int, int>>& getPixels() const;
    static void setMinPixelThreshold(int threshold); // Set the threshold for large"components
};

#endif 