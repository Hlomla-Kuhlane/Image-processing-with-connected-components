#ifndef PGMIMAGEPROCESSOR_H
#define PGMIMAGEPROCESSOR_H

#include "ConnectedComponent.h"
#include "PGMimage.h"
#include <memory>
#include <vector>

class PGMimageProcessor {
private:
    std::vector<std::unique_ptr<ConnectedComponent>> components;
    std::unique_ptr<PGMimage> image;
    int width, height;
    int currentThreshold;
    
    void bfs(int x, int y, std::unique_ptr<ConnectedComponent>& component);
    void thresholdImage(unsigned char threshold);

public:
    PGMimageProcessor(const std::string& filename);
    ~PGMimageProcessor();
    
   
    PGMimageProcessor(const PGMimageProcessor& other) = delete;
    PGMimageProcessor& operator=(const PGMimageProcessor& other) = delete;
    
    
    PGMimageProcessor(PGMimageProcessor&& other) noexcept;
    PGMimageProcessor& operator=(PGMimageProcessor&& other) noexcept;

    int extractComponents(unsigned char threshold, int minValidSize);
    int filterComponentsBySize(int minSize, int maxSize);
    bool writeComponents(const std::string& outFileName);
    
    // Threshold handling
    void setCurrentThreshold(int threshold);
    
    int getComponentCount() const;
    int getLargestSize() const;
    int getSmallestSize() const;
    void printComponentsAboveThreshold() const;
    void printComponentData(const ConnectedComponent& theComponent) const;
};

#endif