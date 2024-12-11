#include "./baseline/baselinelog.h"
#include "./sse/avxlog.h"

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <chrono>
#include <utility>

#define MAX_NUMBER 1000.0

typedef std::chrono::high_resolution_clock::time_point TimeVar;

#define duration(a) std::chrono::duration_cast<std::chrono::nanoseconds>(a).count()
#define timeNow() std::chrono::high_resolution_clock::now()

template<typename F, typename... Args>
double funcTime(F func, Args&&... args) {
    TimeVar t1 = timeNow();
    func(std::forward<Args>(args)...);
    return duration(timeNow() - t1);
}

float * generateInput(int n) {
    float * numbers = new float[n];
    srand(time(NULL));
    for (int i = 0; i < n; ++i) {
        float num = 40 + ((float) rand() / MAX_NUMBER);
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
        std::string input = "1000000";
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
        double baseline = funcTime(baselineVectorLog, experiment1, n);
        double avx = funcTime(avxVectorLog, experiment2, n);
        std::cout << "Done baseline " << baseline << "\n";
        std::cout << "Done AVX " << avx << "\n";
        std::cout << "baseline - avx " << baseline -  avx << "\n";
        if (n <= 16) {
            for (int i = 0; i < n; ++i) {
                std::cout << "data: " << data[i] << ", " << "baseline: " << experiment1[i] << ", avx:  " << experiment2[i] << "\n";
            }
        }
        delete [] experiment1;
        delete [] experiment2;
    } catch (const std::exception& e) {
        std::cout << "Enter number of floats, or a path to data";
    }
    
}