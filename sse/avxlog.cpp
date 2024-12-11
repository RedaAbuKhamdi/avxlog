#include "avxlog.h"

__m256 XS[] = { _mm256_set1_ps(1e-06), _mm256_set1_ps(0.08333429166666667), _mm256_set1_ps(0.16666758333333334), _mm256_set1_ps(0.250000875), _mm256_set1_ps(0.33333416666666665), _mm256_set1_ps(0.41666745833333335), _mm256_set1_ps(0.50000075), _mm256_set1_ps(0.5833340416666667), _mm256_set1_ps(0.6666673333333334), _mm256_set1_ps(0.7500006250000001), _mm256_set1_ps(0.8333339166666668), _mm256_set1_ps(0.9166672083333334), _mm256_set1_ps(1.0000005), _mm256_set1_ps(1.0833337916666665), _mm256_set1_ps(1.1666670833333332), _mm256_set1_ps(1.250000375), _mm256_set1_ps(1.3333336666666666), _mm256_set1_ps(1.4166669583333333), _mm256_set1_ps(1.50000025), _mm256_set1_ps(1.5833335416666667), _mm256_set1_ps(1.6666668333333334), _mm256_set1_ps(1.750000125), _mm256_set1_ps(1.8333334166666666), _mm256_set1_ps(1.9166667083333333), _mm256_set1_ps(2.0) };
__m256 LAGRANGE_COEFFS[] = { _mm256_set1_ps(-1770.1757444324674), _mm256_set1_ps(7641.326498835335), _mm256_set1_ps(-63363.17107054321), _mm256_set1_ps(359512.7647522471), _mm256_set1_ps(-1495762.7806104156), _mm256_set1_ps(4767810.097738895), _mm256_set1_ps(-11953803.739411466), _mm256_set1_ps(23902373.715409547), _mm256_set1_ps(-38209153.45318112), _mm256_set1_ps(48195225.1370086), _mm256_set1_ps(-45816307.48993823), _mm256_set1_ps(27828652.290930226), _mm256_set1_ps(173.24116051446688), _mm256_set1_ps(-25600187.68090697), _mm256_set1_ps(38737374.50501828), _mm256_set1_ps(-37383279.44139729), _mm256_set1_ps(27109916.228098184), _mm256_set1_ps(-15446075.140885122), _mm256_set1_ps(6992544.934971529), _mm256_set1_ps(-2502623.073889431), _mm256_set1_ps(695491.8050672801), _mm256_set1_ps(-145127.5850118051), _mm256_set1_ps(21435.24733475203), _mm256_set1_ps(-2000.6285477979993), _mm256_set1_ps(88.81266611183179) };
__m256 vectorBinaryCoeff[] = { _mm256_set1_ps(1 * LN2), _mm256_set1_ps(2 * LN2), _mm256_set1_ps(3 * LN2), _mm256_set1_ps(4 * LN2), _mm256_set1_ps(5 * LN2), _mm256_set1_ps(6 * LN2), _mm256_set1_ps(7 * LN2), _mm256_set1_ps(8 * LN2), _mm256_set1_ps(9 * LN2), _mm256_set1_ps(10 * LN2), _mm256_set1_ps(11 * LN2), _mm256_set1_ps(12 * LN2), _mm256_set1_ps(13 * LN2), _mm256_set1_ps(14 * LN2), _mm256_set1_ps(15 * LN2), _mm256_set1_ps(16 * LN2), _mm256_set1_ps(17 * LN2), _mm256_set1_ps(18 * LN2), _mm256_set1_ps(19 * LN2), _mm256_set1_ps(20 * LN2), _mm256_set1_ps(21 * LN2), _mm256_set1_ps(22 * LN2), _mm256_set1_ps(23 * LN2), _mm256_set1_ps(24 * LN2), _mm256_set1_ps(25 * LN2), _mm256_set1_ps(26 * LN2), _mm256_set1_ps(27 * LN2), _mm256_set1_ps(28 * LN2), _mm256_set1_ps(29 * LN2), _mm256_set1_ps(30 * LN2), _mm256_set1_ps(31 * LN2), _mm256_set1_ps(32 * LN2) };

void avxVectorLog(float* input, int& n) {
	float two = 2.0;
	__m256 vectorTwos = _mm256_set1_ps(two);
	int binaryCoeff = 0;
	float logValue = 0;
	int interpolationSize = 25;
	int blockSize = 8;
	for (int i = 0; i < std::ceil(n / blockSize); ++i) {
		__m256 vectorized = _mm256_load_ps(input + i * blockSize);
		for (int j = 0; j < 32; j++) {
			vectorized = _mm256_div_ps(vectorized, vectorTwos);
			__m256 condition = _mm256_cmp_ps(vectorized, vectorTwos, _CMP_LT_OS);
			if (_mm256_movemask_ps(condition) == 255) {
				binaryCoeff = j;
				break;
			}
		}
		__m256 numerators[] = { _mm256_sub_ps(vectorized, XS[0]), _mm256_sub_ps(vectorized, XS[1]), _mm256_sub_ps(vectorized, XS[2]), _mm256_sub_ps(vectorized, XS[3]), _mm256_sub_ps(vectorized, XS[4]), _mm256_sub_ps(vectorized, XS[5]), _mm256_sub_ps(vectorized, XS[6]), _mm256_sub_ps(vectorized, XS[7]), _mm256_sub_ps(vectorized, XS[8]), _mm256_sub_ps(vectorized, XS[9]), _mm256_sub_ps(vectorized, XS[10]), _mm256_sub_ps(vectorized, XS[11]), _mm256_sub_ps(vectorized, XS[12]), _mm256_sub_ps(vectorized, XS[13]), _mm256_sub_ps(vectorized, XS[14]), _mm256_sub_ps(vectorized, XS[15]), _mm256_sub_ps(vectorized, XS[16]), _mm256_sub_ps(vectorized, XS[17]), _mm256_sub_ps(vectorized, XS[18]), _mm256_sub_ps(vectorized, XS[19]), _mm256_sub_ps(vectorized, XS[20]), _mm256_sub_ps(vectorized, XS[21]), _mm256_sub_ps(vectorized, XS[22]), _mm256_sub_ps(vectorized, XS[23]), _mm256_sub_ps(vectorized, XS[24]) };
		__m256 result = _mm256_setzero_ps();
		__m256 member = _mm256_set1_ps(1);
		for (int q = 0; q < interpolationSize; q++) {
			member = _mm256_mul_ps(member, numerators[q]);
		}
		for (int k = 0; k < interpolationSize; ++k) {
			result = _mm256_add_ps(
				result, _mm256_mul_ps(
					LAGRANGE_COEFFS[k], _mm256_div_ps(
						member, numerators[k]
					)
				));
		}
		result = _mm256_add_ps(result, vectorBinaryCoeff[binaryCoeff]);
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

