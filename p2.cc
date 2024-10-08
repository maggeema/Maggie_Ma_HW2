#include "image.h"
#include <iostream>
#include <vector>

using namespace std;
using namespace ComputerVisionProjects;

void LabelConnectedComponents(const Image &input_image, Image *output_image) {
    int num_rows = input_image.num_rows();
    int num_columns = input_image.num_columns();

    // Create an output image initialized to 0
    output_image->AllocateSpaceAndSetSize(num_rows, num_columns);
    output_image->SetNumberGrayLevels(255); // Set max gray levels

    vector<vector<int>> label_map(num_rows, vector<int>(num_columns, 0));
    int current_label = 1; // Start labeling from 1

    // First pass: Label components
    for (int row = 0; row < num_rows; ++row) {
        for (int col = 0; col < num_columns; ++col) {
            if (input_image.GetPixel(row, col) == 255 && label_map[row][col] == 0) { // If it's a foreground pixel and not labeled
                // Perform DFS or BFS to label the entire connected component
                // Here we use a simple stack-based DFS for labeling
                vector<pair<int, int>> stack;
                stack.push_back({row, col});
                label_map[row][col] = current_label;

                while (!stack.empty()) {
                    auto [r, c] = stack.back();
                    stack.pop_back();

                    // Check 8-connectivity (up, down, left, right, and diagonals)
                    for (int dr = -1; dr <= 1; ++dr) {
                        for (int dc = -1; dc <= 1; ++dc) {
                            if (dr == 0 && dc == 0) continue; // Skip the current pixel
                            int new_row = r + dr;
                            int new_col = c + dc;

                            if (new_row >= 0 && new_row < num_rows && 
                                new_col >= 0 && new_col < num_columns &&
                                input_image.GetPixel(new_row, new_col) == 255 && 
                                label_map[new_row][new_col] == 0) {
                                label_map[new_row][new_col] = current_label;
                                stack.push_back({new_row, new_col});
                            }
                        }
                    }
                }
                current_label++; // Move to the next label for the next component
            }
        }
    }

    // Second pass: Create the output image based on label_map
    for (int row = 0; row < num_rows; ++row) {
        for (int col = 0; col < num_columns; ++col) {
            if (label_map[row][col] > 0) {
                output_image->SetPixel(row, col, label_map[row][col] * (255 / current_label)); // Normalize to 0-255
            } else {
                output_image->SetPixel(row, col, 0); // Background remains black
            }
        }
    }
}

int main(int argc, char **argv) {
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " {input_binary_image} {output_labeled_image}" << endl;
        return 1;
    }

    const string input_file(argv[1]);
    const string output_file(argv[2]);

    Image input_image;
    if (!ReadImage(input_file, &input_image)) {
        cout << "Can't open file " << input_file << endl;
        return 1;
    }

    Image output_image;
    LabelConnectedComponents(input_image, &output_image);

    if (!WriteImage(output_file, output_image)) {
        cout << "Can't write to file " << output_file << endl;
        return 1;
    }

    cout << "Labeled image saved as: " << output_file << endl;
    return 0;
}
