This File contains Assingment  implements a PGM  image processor that can identify and analyze connected components in grayscale images. The processor can threshold images, find connected components, filter them by size, and output the results.

Components
1. PGMimage Class
Purpose: Handles basic PGM image operations

Key Features:

Reads and writes PGM files (P5 format)

Provides pixel-level access to image data

Manages image memory

2. ConnectedComponent Class
Purpose: Represents a group of connected pixels

Key Features:

Stores pixel coordinates that belong to the component

Tracks component size (pixel count)

Allows setting a minimum size threshold for valid components

3. PGMimageProcessor Class
Purpose: Main processing engine for image analysis

Key Features:

Thresholding images to binary (black/white)

Finding connected components using BFS (Breadth-First Search)

Filtering components by size range

Generating output images with identified components

Reporting component statistics

How It Works
Image Loading: The processor reads a PGM format image file

Thresholding: Converts grayscale to binary using a threshold value

Component Detection: Uses BFS to find all connected white pixel regions

Filtering: Can remove components that are too small or too large

Output: Can generate a new image showing the components and print statistics

Command Line Usage
Copy
./PGMprocessor input.pgm [options]
Options:
-t <value>: Set threshold value (0-255, default: 128)

-m <value>: Set minimum component size to consider (default: 1)

-f <min> <max>: Filter components by size range

-p: Print component data

-w <filename>: Write output image

-h: Show help

Example Commands
Basic component detection:

Copy
./PGMprocessor image.pgm -t 150 -m 50
With filtering and output:

Copy
./PGMprocessor image.pgm -t 200 -m 10 -f 20 1000 -w output.pgm
Print component information:

Copy
./PGMprocessor image.pgm -t 180 -p
Requirements

Notes:

Supports binary PGM (P5) and PPM (P6) formats

4-connected components only (no diagonal connectivity)

For PPM images, converts to grayscale before processing

Maximum image size limited by available memory
