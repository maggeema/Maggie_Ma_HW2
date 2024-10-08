#include "image.h"
#include <cstdio>
#include <iostream>
#include <cmath>
#include <string>
#include <vector>

#define _USE_MATH_DEFINES
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

using namespace std;
using namespace ComputerVisionProjects;

// Function to compute the centroids of labeled objects
vector<pair<int, int>> ComputeCentroids(const Image &image) {
    vector<pair<int, int>> centroids;

    size_t width = image.num_columns();
    size_t height = image.num_rows();

    vector<int> count(256, 0); 
    vector<long long> sumX(256, 0);
    vector<long long> sumY(256, 0);

    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            int label = image.GetPixel(y, x);
            if (label > 0) { // Ignore background
                sumX[label] += x;
                sumY[label] += y;
                count[label]++;
            }
        }
    }

    for (int label = 1; label < count.size(); ++label) {
        if (count[label] > 0) {
            int centroidX = static_cast<int>(sumX[label] / count[label]);
            int centroidY = static_cast<int>(sumY[label] / count[label]);
            centroids.push_back(make_pair(centroidX, centroidY));
        }
    }
    return centroids;
}

// Function to compute the moment of inertia and draw orientation lines
void ComputeAndDrawOrientation(const vector<pair<int, int>> &centroids, const Image &image, Image &output_image) {
    for (const auto &centroid : centroids) {
        int cx = centroid.first;
        int cy = centroid.second;

        // Calculate second moments and cross moments (this is a simplified example)
        double a = 0, b = 0, c = 0;
        for (int y = 0; y < image.num_rows(); ++y) {
            for (int x = 0; x < image.num_columns(); ++x) {
                int label = image.GetPixel(y, x);
                if (label > 0) { 
                    // Ignore background
                    double dx = x - cx;
                    double dy = y - cy;
                    a += dy * dy;
                    b += -2 * dx * dy;
                    c += dx * dx;
                }
            }
        }

        // Calculate orientation using moments
        const double theta1 = atan2(b, a - c) / 2.0;
        const double theta2 = theta1 + M_PI / 2.0;

        // Draw the orientation line from centroid
        int line_length = 30;
        int x_end = cx + static_cast<int>(line_length * cos(theta1));
        int y_end = cy + static_cast<int>(line_length * sin(theta1));

        // Draw line starting from centroid and extending along the calculated orientation
        // 255 = white line
        DrawLine(cx, cy, x_end, y_end, 255, &output_image); 
    }
}

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Usage: %s input_file output_file\n", argv[0]);
        return 0;
    }
    const string input_file(argv[1]);
    const string output_file(argv[2]);

    Image an_image;
    if (!ReadImage(input_file, &an_image)) {
        cout << "Can't open file " << input_file << endl;
        return 0;
    }

    // Create a copy for output
    Image output_image = an_image; 

    // Compute centroids
    vector<pair<int, int>> centroids = ComputeCentroids(an_image);

    // Draw centroids and orientation lines on the image
    ComputeAndDrawOrientation(centroids, an_image, output_image);

    // Write the modified image to output
    if (!WriteImage(output_file, output_image)) {
        cout << "Can't write to file " << output_file << endl;
        return 0;
    }

    cout << "Output written to " << output_file << endl;
    return 0;
}
