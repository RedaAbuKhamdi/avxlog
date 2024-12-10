#include "baselinelog.h"

void baselineVectorLog(float * input, int &n) {
    for (int i = 0; i < n; ++i) {
        input[i] = log(input[i]);
    }
}