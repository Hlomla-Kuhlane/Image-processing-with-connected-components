#ifndef PGMIMAGE_H
#define PGMIMAGE_H

#include <string>

class PGMimage {
protected:
    unsigned char* buffer;
    int width;
    int height;

public:
    PGMimage();
    virtual ~PGMimage();
    
    // Image dimensions
    int getWidth() const;
    int getHeight() const;
    
    // Pixel access
    virtual unsigned char getPixel(int x, int y) const;
    virtual void setPixel(int x, int y, unsigned char value);
    
    // File operations
    virtual void read(const std::string& fileName);
    virtual void write(const std::string& fileName);
    
    // Image data management
    void setImageData(unsigned char* data, int wd, int ht);
};

#endif