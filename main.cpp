#define DOCTEST_CONFIG_IMPLEMENT
#include "include/doctest.h"
#include "cosine.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>

// Read a single vector
std::vector<double> read_vector(std::istream& in_stream) {
    std::vector<double> res;
    double value;
    while (in_stream >> value) {
        res.push_back(value);
    }
    return res;
}

// Read a matrix of vectors
std::vector<std::vector<double>> read_matrix(std::istream& in_stream) {
    std::vector<std::vector<double>> matrix;
    std::string line;
    while (std::getline(in_stream, line)) {
        if (line.empty())
            continue;
        std::istringstream is_line(line);
        matrix.push_back(read_vector(is_line));
    }
    return matrix;
}

int runMainProgram(int argc, char* argv[]) {
    std::istream* inStream = &std::cin;
    std::ifstream fileStream;

    // Open file if given
    if (argc > 1) {
        fileStream.open(argv[1]);
        if (!fileStream) {
            std::cerr << "Error opening file: " << argv[1] << std::endl;
            return 1;
        }
        inStream = &fileStream;
    }

    auto matrix = read_matrix(*inStream);

    if (matrix.size() < 2) {
        std::cerr << "Need at least two vectors" << std::endl;
        return 1;
    }

    size_t vecSize = matrix[0].size();
    for (size_t i = 1; i < matrix.size(); i++) {
        if (matrix[i].size() != vecSize) {
            std::cerr << "All vectors must be the same size" << std::endl;
            return 1;
        }
    }

    // Compute all distances
    std::vector<PairDistance> pairDistances;
    for (size_t i = 0; i < matrix.size(); i++) {
        for (size_t j = i + 1; j < matrix.size(); j++) {
            double distance = cosineDistance(matrix[i], matrix[j]);
            pairDistances.push_back({ static_cast<int>(i), static_cast<int>(j), distance });
        }
    }

    // Sort pairs
    std::sort(pairDistances.begin(), pairDistances.end(),
              [](const PairDistance& a, const PairDistance& b) {
                  return a.distance < b.distance;
              });

    // Set precision
    std::cout << std::fixed << std::setprecision(4);

    // Output each pair
    for (const auto& pd : pairDistances) {
        std::cout << "Pair (" << pd.i << ", " << pd.j << "): Cosine Distance = " << pd.distance << "\n";
        std::cout << "Vector " << pd.i << ": " << matrix[pd.i] << "\n";
        std::cout << "Vector " << pd.j << ": " << matrix[pd.j] << "\n\n";
    }

    return 0;
}

// Test cosineDistance
TEST_CASE("cosineDistance") {
    SUBCASE("Identical vectors = 0") {
        std::vector<double> v1 = {1.0, 2.0, 3.0};
        std::vector<double> v2 = {1.0, 2.0, 3.0};
        CHECK(cosineDistance(v1, v2) == doctest::Approx(0.0));
    }
    SUBCASE("Opposite vectors = 2") {
        std::vector<double> v1 = {1.0, 2.0, 3.0};
        std::vector<double> v2 = {-1.0, -2.0, -3.0};
        CHECK(cosineDistance(v1, v2) == doctest::Approx(2.0));
    }
    SUBCASE("Zero vector = distance 1") {
        std::vector<double> v1 = {0.0, 0.0, 0.0};
        std::vector<double> v2 = {1.0, 2.0, 3.0};
        CHECK(cosineDistance(v1, v2) == doctest::Approx(1.0));
    }
}

// Test read_vector
TEST_CASE("Reading vector") {
    std::istringstream iss("1.0 2.0 3.0");
    auto vec = read_vector(iss);
    CHECK(vec.size() == 3);
    CHECK(vec[0] == doctest::Approx(1.0));
    CHECK(vec[1] == doctest::Approx(2.0));
    CHECK(vec[2] == doctest::Approx(3.0));
}

// Test read_matrix
TEST_CASE("Reading matrix") {
    std::istringstream iss("1.0 2.0 3.0\n4.0 5.0 6.0\n7.0 8.0 9.0");
    auto matrix = read_matrix(iss);
    CHECK(matrix.size() == 3);
    CHECK(matrix[0] == std::vector<double>{1.0, 2.0, 3.0});
    CHECK(matrix[1] == std::vector<double>{4.0, 5.0, 6.0});
    CHECK(matrix[2] == std::vector<double>{7.0, 8.0, 9.0});
}

// Test overloaded operator<<
TEST_CASE("operator<< outputs vector correctly") {
    std::vector<double> vec = {1.0, 2.0, 3.0};
    std::ostringstream oss;
    oss << vec;
    CHECK(oss.str() == "1 2 3 ");
}

int main(int argc, char* argv[]) {
    // Run main
    int prodResult = runMainProgram(argc, argv);

    // Run tests
    doctest::Context tests;
    tests.applyCommandLine(argc, argv);
    int testResult = tests.run(); // run all tests

    // Return non-zero if anything fails
    return (prodResult != 0 || testResult != 0) ? 1 : 0;
}
