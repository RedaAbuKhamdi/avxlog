#pragma once
#include <immintrin.h>
#include <cmath>
#include <iostream>

#define EPSILON 1e-11
#define LN2 0.69314718055994

void avxVectorLog(float* input, int& n);
