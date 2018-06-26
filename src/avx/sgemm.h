#pragma once

/*
 * Performs single-float matrix multiply C = A * B in row-major fashion,
 * where C is MxN, A is MxK and B is KxN.
 * Allocation requirement: packA: M * K
 */

template<int ROW_BATCH>
void packed_sgemm_init(int M, int K, int kc, float* packA, float* A, int lda);

template<bool fuseBias, bool fuseRelu>
void packed_sgemm_activation(int M, int N, int K, float *packA, float *b, int ldb, float *c, int ldc, int nc, int kc, float* bias);
