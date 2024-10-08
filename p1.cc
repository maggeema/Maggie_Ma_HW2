#include "image.h"
#include <cstdio>
#include <iostream>
#include <string>

using namespace std;
using namespace ComputerVisionProjects;

int main(int argc, char **argv) {
  
  if (argc != 4) {
    printf("Usage: %s {input gray-level image} {input gray-level threshold} {output binary image}\n", argv[0]);
    return 0;
  }

  const string input_file(argv[1]);
  const int threshold = stoi(argv[2]);  // Convert threshold to an integer.
  const string output_file(argv[3]);

  // Read input image
  Image an_image;
  if (!ReadImage(input_file, &an_image)) {
    cout << "Can't open file " << input_file << endl;
    return 0;
  }

  // Apply thresholding to convert the grayscale image to binary
  for (size_t i = 0; i < an_image.num_rows(); ++i) {
    for (size_t j = 0; j < an_image.num_columns(); ++j) {
      int pixel_value = an_image.GetPixel(i, j);
      
      // Set the pixel to 0 (black) or 255 (white) based on the threshold
      if (pixel_value >= threshold) {
        an_image.SetPixel(i, j, 255);  // white
      } else {
        an_image.SetPixel(i, j, 0);    // black
      }
    }
  }

  // Write the binary image to the output file
  if (!WriteImage(output_file, an_image)) {
    cout << "Can't write to file " << output_file << endl;
    return 0;
  }

  return 0;
}
