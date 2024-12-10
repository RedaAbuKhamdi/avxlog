#include "./baseline/baselinelog.h"
#include "./sse/avxlog.h"

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

#define MAX_NUMBER 1000.0

float * generateInput(int n) {
    float * numbers = new float[n];
    srand(time(NULL));
    for (int i = 0; i < n; ++i) {
        float num = 1.1 + ((float) rand() / MAX_NUMBER);
        numbers[i] = num;
    }
    return numbers;
}

float * generateInput(int & n, std::string path) {
    std::string line;
    std::ifstream inputFile(path);
    std::getline (inputFile, line);
    n = std::stoi(line);
    float * numbers = new float[n];
    for (int i = 0; i < n; ++i) {
        std::getline (inputFile, line);
        numbers[i] = std::stof(line);
    }
    return numbers;
}

int main(int argc, char *argv[]) {
    try {
        std::string input = "16";
        float* data;
        int n;
        try {
            n = std::stoi(input);
            data = generateInput(n);
        } catch(const std::exception& e) {
            data = generateInput(n, input);
        }
        float* experiment1 = new float[n];
        float* experiment2 = new float[n];
        memcpy(experiment1, data, n * sizeof(float));
        memcpy(experiment2, data, n * sizeof(float));

        baselineVectorLog(experiment1, n);
        std::cout << "Done baseline" << "\n";
        avxVectorLog(experiment2, n);
        for (int i = 0; i < n; ++i) {
            std::cout << "data: " << data[i] << ", " << "baseline: " << experiment1[i] << ", avx:  " << experiment2[i] << "\n";
        }
        delete [] experiment1;
        delete [] experiment2;
    } catch (const std::exception& e) {
        std::cout << "Enter number of floats, or a path to data";
    }
    
}