#include "avxlog.h"

void avxVectorLog(float* input, int& n) {
	float two = 2.0;
	__m256 vectorTwos = _mm256_set1_ps(two);
	__m256 vectorOnes = _mm256_set1_ps(1.0);
	int binaryCoeff = 0;
	float logValue = 0;
	int blockSize = 8;
	for (int i = 0; i < std::ceil(n / blockSize); ++i) {
		__m256 vectorized = _mm256_load_ps(input + i * blockSize);
		for (int j = 0; j < 32; j++) {
			vectorized = _mm256_div_ps(vectorized, vectorTwos);
			__m256 condition = _mm256_cmp_ps(vectorized, vectorOnes, 1);
			if (_mm256_movemask_ps(condition) == 255) {
				binaryCoeff = j + 1;
			}
		}
		binaryCoeff = binaryCoeff * LN2;
		__m256 result = _mm256_setzero_ps();
		__m256 vectorBinaryCoeff = _mm256_set1_ps(binaryCoeff);
		for (int k = 1; k < 6; ++k) {
			if (k % 2) {
				result = _mm256_add_ps(result, vectorized);
			}
			else {
				result = _mm256_sub_ps(result, vectorized);
			}
			vectorized = _mm256_mul_ps(vectorized, vectorized);
		}
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

