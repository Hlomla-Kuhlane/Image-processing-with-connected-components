#include "PGMimage.h"
#include <fstream>
#include <sstream>
#include <iostream>

PGMimage::PGMimage() : buffer(nullptr), width(0), height(0) {}

PGMimage::~PGMimage() {
    if (buffer) delete[] buffer;
}

int PGMimage::getWidth() const { return width; }
int PGMimage::getHeight() const { return height; }

unsigned char PGMimage::getPixel(int x, int y) const {
    if (!buffer || x < 0 || x >= width || y < 0 || y >= height)
        return 0;
    return buffer[y * width + x];
}

void PGMimage::setPixel(int x, int y, unsigned char value) {
    if (buffer && x >= 0 && x < width && y >= 0 && y < height)
        buffer[y * width + x] = value;
}

void PGMimage::read(const std::string& fileName) {
    std::ifstream ifs(fileName, std::ios::binary);
    if (!ifs) {
        std::cerr << "Failed to open file for read: " << fileName << std::endl;
        return;
    }

    std::string line;
    ifs >> line >> std::ws;
    if (line != "P5") {
        std::cerr << "Malformed PGM file - magic is: " << line << std::endl;
        return;
    }

    while (std::getline(ifs, line)) {
        if (line[0] != '#') break;
    }

    std::istringstream iss(line);
    iss >> width >> height;
    if (!iss) {
        std::cerr << "Header not correct - unexpected image sizes found: " << line << std::endl;
        return;
    }

    int maxChan = 0;
    ifs >> maxChan >> std::ws;
    if (maxChan != 255) {
        std::cerr << "Max grey level incorrect - found: " << maxChan << std::endl;
    }

    if (buffer) delete[] buffer;
    buffer = new unsigned char[width * height];
    ifs.read(reinterpret_cast<char*>(buffer), width * height);

    if (!ifs) {
        std::cerr << "Failed to read binary block" << std::endl;
        delete[] buffer;
        buffer = nullptr;
        width = height = 0;
    }

    ifs.close();
}

void PGMimage::write(const std::string& fileName) {
    if (!buffer || width < 1 || height < 1) {
        std::cerr << "Invalid data for PGM write to " << fileName << std::endl;
        return;
    }

    std::ofstream ofs(fileName, std::ios::binary);
    if (!ofs) {
        std::cerr << "Unable to open PGM output file " << fileName << std::endl;
        return;
    }

    ofs << "P5\n# Created by PGMimageProcessor\n" 
        << width << " " << height << "\n255\n";
    ofs.write(reinterpret_cast<char*>(buffer), width * height);

    if (!ofs) {
        std::cerr << "Error writing binary block of PGM" << std::endl;
    }

    ofs.close();
}

void PGMimage::setImageData(unsigned char* data, int wd, int ht) {
    if (data == nullptr || wd < 1 || ht < 1) {
        std::cerr << "setImageData() invalid data specified - aborted.\n";
        return;
    }
    if (buffer) delete[] buffer;
    buffer = new unsigned char[wd * ht];
    width = wd; 
    height = ht;
    for (int i = 0; i < wd * ht; ++i) 
        buffer[i] = data[i];
}