#ifndef COSINE_H
#define COSINE_H
#include <vector>
#include <iostream>
#include <string>

std::ostream& operator<<(std::ostream& os, const std::vector<double>& vec);

double cosineDistance(const std::vector<double>& v1, const std::vector<double>& v2);

// Structure to hold pair of vectors
struct PairDistance {
    int i;
    int j;
    double distance;
};

std::vector<std::vector<double>> readVectors(std::istream& inStream);

#endif
