#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() function
#include "catch.hpp"
#include "PGMimageProcessor.h"
#include "ConnectedComponent.h"
#include <fstream>
#include <sstream>
#include <filesystem>

// Helper function to check if file exists
bool fileExists(const std::string& filename) {
    return std::filesystem::exists(filename);
}

TEST_CASE("PGMimageProcessor move operations", "[move]") {
    SECTION("Move constructor") {
        PGMimageProcessor original("Birds.pgm");
        original.extractComponents(150, 1);
        int originalCount = original.getComponentCount();
        
        PGMimageProcessor moved(std::move(original));
        
        REQUIRE(moved.getComponentCount() == originalCount);
        REQUIRE(original.getComponentCount() == 0); // original should be empty after move
    }
    
    SECTION("Move assignment") {
        PGMimageProcessor processor1("Birds.pgm");
        processor1.extractComponents(35, 1);
        int count = processor1.getComponentCount();
        
        PGMimageProcessor processor2("Birds.pgm");
        processor2 = std::move(processor1);
        
        REQUIRE(processor2.getComponentCount() == count);
        REQUIRE(processor1.getComponentCount() == 0); // source should be empty after move
    }
}
//Test case for extractComponents method
TEST_CASE("PGMimageProcessor extractComponents", "[extract]") {
    PGMimageProcessor processor("Birds.pgm");
    
    SECTION("Basic extraction") {
        int count = processor.extractComponents(35, 1);
        REQUIRE(count > 0);
        REQUIRE(processor.getComponentCount() > 0);
    }
    
    SECTION("Threshold effect") {
        PGMimageProcessor processor1("Birds.pgm");
        int countLow = processor1.extractComponents(35, 1);
        
        PGMimageProcessor processor2("Birds.pgm");
        int countHigh = processor2.extractComponents(250, 1);
        
        REQUIRE(countLow > countHigh);
    }
    
    SECTION("Minimum size filtering") {
        int countWithMinSize = processor.extractComponents(35, 1);
        
        PGMimageProcessor processor2("Birds.pgm");
        int countNoMinSize = processor2.extractComponents(35, 1);
        REQUIRE(countNoMinSize >= countWithMinSize);
    }
}

// Test case for filterComponentsBySize method
TEST_CASE("PGMimageProcessor filterComponentsBySize", "[filter]") {
    PGMimageProcessor processor("Birds.pgm");
    processor.extractComponents(35, 1);
    int initialCount = processor.getComponentCount();
   
    SECTION("Basic filtering") {
        int remainingCount = processor.filterComponentsBySize(35, 1000);
        REQUIRE(remainingCount <= initialCount);
    }
    
    SECTION("Filter all components") {
        int remainingCount = processor.filterComponentsBySize(1000, 2000);
        REQUIRE(remainingCount == 0);
    }
    
    SECTION("Keep all components") {
        int remainingCount = processor.filterComponentsBySize(1, 10000);
        REQUIRE(remainingCount == initialCount);
    }
}

// Test case for writeComponents method
TEST_CASE("PGMimageProcessor writeComponents", "[write]") {
    PGMimageProcessor processor("Birds.pgm");
    processor.extractComponents(35, 1);
    
    SECTION("Write components to file") {
        REQUIRE(processor.writeComponents("output_test.pgm"));
        REQUIRE(fileExists("output_test.pgm"));
        
        // Clean up
        std::remove("output_test.pgm");
    }
}

// Test case for component size methods
TEST_CASE("PGMimageProcessor component size methods", "[sizes]") {
    PGMimageProcessor processor("Birds.pgm");
    processor.extractComponents(35, 1);
    
    SECTION("Size relationships") {
        REQUIRE(processor.getLargestSize() >= processor.getSmallestSize());
    }
}

// Test case for error handling
TEST_CASE("PGMimageProcessor error handling", "[errors]") {
    SECTION("Invalid filename") {
        PGMimageProcessor processor("nonexistent_file.pgm");
        int count = processor.extractComponents(150, 1);
        REQUIRE(count == 0);
    }
}