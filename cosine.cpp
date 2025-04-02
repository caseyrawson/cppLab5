#include "cosine.h"
#include <sstream>
#include <cmath>
#include <algorithm>

// Operator<< for printing a vector
std::ostream& operator<<(std::ostream& os, const std::vector<double>& vec) {
    for (const double& d : vec) {
        os << d << " ";
    }
    return os;
}

// Compute cosine distance
double cosineDistance(const std::vector<double>& v1, const std::vector<double>& v2) {
    double dot = 0.0, mag1 = 0.0, mag2 = 0.0;
    for (size_t i = 0; i < v1.size(); i++) {
        dot += v1[i] * v2[i];
        mag1 += v1[i] * v1[i];
        mag2 += v2[i] * v2[i];
    }
    // If zero magnitude, distance = 1
    if (mag1 == 0.0 || mag2 == 0.0)
        return 1.0;
    double cosineSim = dot / (std::sqrt(mag1) * std::sqrt(mag2));
    return 1.0 - cosineSim;
}

// Read vectors from input
std::vector<std::vector<double>> readVectors(std::istream& inStream) {
    std::vector<std::vector<double>> vectors;
    std::string line;
    while (std::getline(inStream, line)) {
        if (line.empty()) continue;
        std::istringstream input(line);
        std::vector<double> vec;
        double value;
        while (input >> value) {
            vec.push_back(value);
        }
        if (!vec.empty())
            vectors.push_back(vec);
    }
    return vectors;
}
