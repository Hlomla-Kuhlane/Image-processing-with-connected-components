#include "PGMimageProcessor.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

int main(int argc, char* argv[]) {
    // Check if user provided at least one argument 
    if (argc < 2) {
        std::cout << "Enter correct input";
        return 1;
    }

    // Store command line arguments in a vector for easier processing
    std::vector<std::string> args(argv + 1, argv + argc);
    
    // Variables to store user inputs with default values
    std::string inputFile;         
    std::string outputFile;        
    int threshold = 128;           
    int minValidSize = 1;          // Minimum size of a component to consider
    int printThreshold = 0;        // Threshold for printing component info
    int filterMin = -1, filterMax = -1;  // For filtering components by size (-1 means not set)
    bool printData = false;        // Flag to print component data
    bool writeOutput = false;      // Flag to save output image

    // Process each command line argument
    for (size_t i = 0; i < args.size(); i++) {
        if (args[i] == "-h") {
            // Show help message (implementation not shown here)
            return 0;
        }
        // Set brightness threshold 
        else if (args[i] == "-t" && i + 1 < args.size()) {
            threshold = std::stoi(args[++i]);
            // Ensure threshold is between 0-255
            threshold = std::max(0, std::min(255, threshold));
        }
        // Set minimum component size 
        else if (args[i] == "-m" && i + 1 < args.size()) {
            minValidSize = std::stoi(args[++i]);
            minValidSize = std::max(1, minValidSize);
            printThreshold = minValidSize; 
        }
        // Filter components by size range 
        else if (args[i] == "-f" && i + 2 < args.size()) {
            filterMin = std::stoi(args[++i]);
            filterMax = std::stoi(args[++i]);
        }
        // Flag to print component data 
        else if (args[i] == "-p") {
            printData = true;
        }
        // Save output image 
        else if (args[i] == "-w" && i + 1 < args.size()) {
            writeOutput = true;
            outputFile = args[++i];
        }
        // If argument doesn't start with -, assume it's the input filename
        else if (args[i][0] != '-') {
            inputFile = args[i];
        }
    }

    // Check if input file was provided
    if (inputFile.empty()) {
        std::cerr << "Error: No input file specified\n";
        return 1;
    }

    try {
        // Create image processor with input file
        PGMimageProcessor processor(inputFile);
        
        // Set the size threshold for printing components
        processor.setCurrentThreshold(printThreshold);
        
        // Find all components in the image
        int initialCount = processor.extractComponents(threshold, minValidSize);
        std::cout << "Found " << initialCount << " components initially\n";
        
        // Filter components by size if requested
        if (filterMin != -1 && filterMax != -1) {
            int filteredCount = processor.filterComponentsBySize(filterMin, filterMax);
            std::cout << "After filtering: " << filteredCount << " components remain\n";
        }

        // Print component data if requested
        if (printData) {
            processor.setCurrentThreshold(threshold);
            std::cout << "\nComponents above (" << threshold << "):\n";
            std::cout << "Components required : " << processor.getComponentCount() << "\n";
            processor.printComponentsAboveThreshold();
        }
    
        // Save output image 
        if (writeOutput) {
            if (processor.writeComponents(outputFile)) {
                std::cout << "Output written to " << outputFile << std::endl;
            } else {
                std::cerr << "Failed to write output file\n";
            }
        }

    } catch (const std::exception& e) {
        // Handle any errors that occur
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}