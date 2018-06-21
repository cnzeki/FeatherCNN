#pragma once

/*
 * Performs single-float matrix multiply C = A * B in row-major fashion,
 * where C is MxN, A is MxK and B is KxN.
 * Allocation requirement: C: get_aligned_size(M, N), A: get_aligned_size(M, K)
 */

int get_aligned_size(int M, int N);

template<int ROW_BATCH>
void packed_sgemm_init(int M, int K, int kc, float* packA, float* A, int lda);

void packed_sgemm(int M, int N, int K, float *packA, float *B, int ldb, float *C, int ldc, int nc, int kc);
