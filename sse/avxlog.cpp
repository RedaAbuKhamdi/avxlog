#include "avxlog.h"

void avxVectorLog(float* input, int& n) {
	float two = 2.0;
	__m256 vectorTwos = _mm256_set1_ps(two);
	__m256 vectorOnes = _mm256_set1_ps(1.0);
	int binaryCoeff = 0;
	float logValue = 0;
	int blockSize = 8;
	for (int i = 0; i < std::ceil(n / blockSize); ++i) {
		std::cout << "ITER" << "\n";
		__m256 vectorized = _mm256_loadu_ps(input + i * blockSize);
		__m256 result = _mm256_setzero_ps();
		__m256 numerator = _mm256_sub_ps(vectorized, vectorOnes);
		__m256 denominator = _mm256_add_ps(vectorized, vectorOnes);
		vectorized = _mm256_div_ps(numerator, denominator);
		for (int k = 0; k < 10000; ++k) {
			result = _mm256_add_ps(result, _mm256_div_ps(vectorized, _mm256_set1_ps(2.0*k + 1.0)));
			vectorized = _mm256_mul_ps(vectorized, vectorized);
			vectorized = _mm256_mul_ps(vectorized, vectorized);
		}
		result = _mm256_mul_ps(result, vectorTwos);
		float* resultArray = (float*)&result;
		input[i * blockSize] = resultArray[0];
		input[i * blockSize + 1] = resultArray[1];
		input[i * blockSize + 2] = resultArray[2];
		input[i * blockSize + 3] = resultArray[3];
		input[i * blockSize + 4] = resultArray[4];
		input[i * blockSize + 5] = resultArray[5];
		input[i * blockSize + 6] = resultArray[6];
		input[i * blockSize + 7] = resultArray[7];
	}
}

