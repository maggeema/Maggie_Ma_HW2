#include <iostream>
#include <vector>
#include <queue>
#include "image.h"  // Include your header file for the Image class

using namespace std;
using namespace ComputerVisionProjects;

void labelRegions(const Image &binary_image, Image &labeled_image) {
    int width = binary_image.num_columns();
    int height = binary_image.num_rows();
    
    // Create an empty labeled image using the default constructor
    labeled_image.AllocateSpaceAndSetSize(height, width); // Allocate space for the labeled image

    vector<vector<int>> labels(height, vector<int>(width, 0));
    int label = 1;  // Start labeling from 1 (or a non-zero value)

    // Directions for 4-connected neighbors
    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    vector<vector<bool>> visited(height, vector<bool>(width, false));

    // First pass: Label connected components
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (binary_image.GetPixel(i, j) == 255 && !visited[i][j]) {
                // Start a BFS for a new connected component
                queue<pair<int, int>> q;
                q.push({i, j});
                visited[i][j] = true;

                while (!q.empty()) {
                    auto [x, y] = q.front();
                    q.pop();
                    labels[x][y] = label;  // Assign label

                    for (auto &dir : directions) {
                        int new_x = x + dir[0];
                        int new_y = y + dir[1];

                        if (new_x >= 0 && new_x < height && new_y >= 0 && new_y < width &&
                            binary_image.GetPixel(new_x, new_y) == 255 && !visited[new_x][new_y]) {
                            visited[new_x][new_y] = true;
                            q.push({new_x, new_y});
                        }
                    }
                }
                label++;  // Increment the label for the next component
            }
        }
    }

    // Second pass: Create the labeled image
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (labels[i][j] > 0) {
                // Scale the label for visibility, ensuring it remains within valid range
                labeled_image.SetPixel(i, j, (labels[i][j] * (255 / label))); 
            } else {
                labeled_image.SetPixel(i, j, 0); // Background
            }
        }
    }
}

int main(int argc, char **argv) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " {input binary image} {output labeled image}" << endl;
        return 1;
    }

    const string input_file(argv[1]);
    const string output_file(argv[2]);

    // Read input binary image
    Image binary_image;
    if (!ReadImage(input_file, &binary_image)) {
        cerr << "Can't open file " << input_file << endl;
        return 1;
    }

    // Prepare output labeled image
    Image labeled_image; // Use the default constructor

    // Label regions in the binary image
    labelRegions(binary_image, labeled_image);

    // Write the labeled image to the output file
    if (!WriteImage(output_file, labeled_image)) {
        cerr << "Can't write to file " << output_file << endl;
        return 1;
    }

    cout << "Processing completed successfully!" << endl;

    return 0;
}
