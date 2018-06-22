#include "sgemm.h"
#include <immintrin.h>
#include <string.h>


int align_ceil(int num, int align)
{
	return num + (align - (num % align)) % align;
}

void inner_kernel_6x16(int K, float *packA, float *packB, float *c, int ldc)
{
	float *aptr = packA;
	float *bptr = packB;
	float *cptr = c;
	__m256 va, va1, va2, va3, va4, va5;
	__m256 vb0, vb1, vb2, vB0, vB1, vB2;
	__m256 vc0, vc1, vc2, vc3, vc4, vc5, vc6, vc7, vc8, vc9, vcA, vcB;

	vc0 = _mm256_load_ps(cptr); 
	vc6 = _mm256_load_ps(cptr + 8);
	cptr += ldc;

	vc1 = _mm256_load_ps(cptr);
	vc7 = _mm256_load_ps(cptr + 8);
	cptr += ldc;

	vc2 = _mm256_load_ps(cptr);
	vc8 = _mm256_load_ps(cptr + 8);

	cptr += ldc;
	vc3 = _mm256_load_ps(cptr);
	vc9 = _mm256_load_ps(cptr + 8);

	cptr += ldc;
	vc4 = _mm256_load_ps(cptr);
	vcA = _mm256_load_ps(cptr + 8);

	cptr += ldc;
	vc5 = _mm256_load_ps(cptr);
	vcB = _mm256_load_ps(cptr + 8);

	vb0 = _mm256_load_ps(bptr);
	vb1 = _mm256_load_ps(bptr + 8);
	for(int p = 0; p < (K-1); ++p){
		va = _mm256_broadcast_ss(aptr);
		vc0 = _mm256_fmadd_ps(vb0, va, vc0);
		vc6 = _mm256_fmadd_ps(vb1, va, vc6);

		va = _mm256_broadcast_ss(aptr+1);
		vc1 = _mm256_fmadd_ps(vb0, va, vc1);
		vc7 = _mm256_fmadd_ps(vb1, va, vc7);

		va = _mm256_broadcast_ss(aptr+2);
		vc2 = _mm256_fmadd_ps(vb0, va, vc2);
		vc8 = _mm256_fmadd_ps(vb1, va, vc8);

		va = _mm256_broadcast_ss(aptr+3);
		vc3 = _mm256_fmadd_ps(vb0, va, vc3);
		vc9 = _mm256_fmadd_ps(vb1, va, vc9);

		va = _mm256_broadcast_ss(aptr+4);
		vc4 = _mm256_fmadd_ps(vb0, va, vc4);
		vcA = _mm256_fmadd_ps(vb1, va, vcA);


		va = _mm256_broadcast_ss(aptr+5);
		vc5 = _mm256_fmadd_ps(vb0, va, vc5);
		vcB = _mm256_fmadd_ps(vb1, va, vcB);

		vb0 = _mm256_load_ps(bptr + 16);
		vb1 = _mm256_load_ps(bptr + 24);
		bptr+=16;
		aptr+=6;
	}
	va = _mm256_broadcast_ss(aptr);
	vc0 = _mm256_fmadd_ps(vb0, va, vc0);
	vc6 = _mm256_fmadd_ps(vb1, va, vc6);

	va = _mm256_broadcast_ss(aptr+1);
	vc1 = _mm256_fmadd_ps(vb0, va, vc1);
	vc7 = _mm256_fmadd_ps(vb1, va, vc7);

	va = _mm256_broadcast_ss(aptr+2);
	vc2 = _mm256_fmadd_ps(vb0, va, vc2);
	vc8 = _mm256_fmadd_ps(vb1, va, vc8);

	va = _mm256_broadcast_ss(aptr+3);
	vc3 = _mm256_fmadd_ps(vb0, va, vc3);
	vc9 = _mm256_fmadd_ps(vb1, va, vc9);

	va = _mm256_broadcast_ss(aptr+4);
	vc4 = _mm256_fmadd_ps(vb0, va, vc4);
	vcA = _mm256_fmadd_ps(vb1, va, vcA);

	va = _mm256_broadcast_ss(aptr+5);
	vc5 = _mm256_fmadd_ps(vb0, va, vc5);
	vcB = _mm256_fmadd_ps(vb1, va, vcB);


	cptr = c;
	_mm256_store_ps(cptr, vc0);
	_mm256_store_ps(cptr + 8, vc6);

	cptr += ldc;
	_mm256_store_ps(cptr, vc1);
	_mm256_store_ps(cptr + 8, vc7);

	cptr += ldc;
	_mm256_store_ps(cptr, vc2);
	_mm256_store_ps(cptr + 8, vc8);

	cptr += ldc;
	_mm256_store_ps(cptr, vc3);
	_mm256_store_ps(cptr + 8, vc9);

	cptr += ldc;
	_mm256_store_ps(cptr, vc4);
	_mm256_store_ps(cptr + 8, vcA);

	cptr += ldc;
	_mm256_store_ps(cptr, vc5);
	_mm256_store_ps(cptr + 8, vcB);
}

inline void compute_block(int M, int nc, int kc, float* packA, float* packB, float* loadC, float *C, int ldc)
{
	//M is already aligned. 
	int nc_aligned = align_ceil(nc, 16);
	memset(loadC, 0, 6*nc*sizeof(float));
	for(int i = 0; i < M; i += 6)
	{
		//Load C into cache
		float* rC = C + i * ldc;
		for(int m = 0; m < 6; ++m)
		{
			//for(int n = 0; n < nc; ++n)
			//{
			//	loadC[m * nc_aligned + n] = rC[m * ldc + n];
			//}
			float* pC = rC + m * ldc;
			float* pL = loadC + m * nc_aligned;
			for(int n = 0; n < nc; n += 8)
			{
				_mm256_store_ps(pL, _mm256_load_ps(pC));
				pC += 8;
				pL += 8;
			}
			for(int n = nc - nc % 8; n < nc; ++n)
			{
				pL[n] = pC[n];
			}
		}
		for(int j = 0; j < nc_aligned; j+=16)
		{
			float* pC = loadC + j;
			float* pA = packA + i * kc;
			float* pB = packB + j * kc;
			inner_kernel_6x16(kc, pA, pB, pC, nc_aligned);
				
		}
		//Write Results
		for(int m = 0; m < 6; ++m)
		{
			float* pC = rC + m * ldc;
			float* pL = loadC + m * nc_aligned;
			for(int n = 0; n < nc; n += 8)
			{
				_mm256_store_ps(pC + n, _mm256_load_ps(pL + n));
			}
			for(int n = nc - nc % 8; n < nc; ++n)
			{
				pC[n] = pL[n];
			}
		}
	}
}

int get_aligned_size(int M, int K)
{
	const int ROW_BATCH = 6;
	return (M + (ROW_BATCH - (M % ROW_BATCH)) % ROW_BATCH) * K;
}

//Decide how many rows should be packed together.
template<int ROW_BATCH>
void packed_sgemm_init(int M, int K, int kc, float* packA, float* A, int lda)
{
	int M_align = align_ceil(M, 6);
	for(int p = 0; p < K; p += kc)
	{
		
		//The last row batch may not have sufficient rows
		//Implicit padding so as to reduce code complexity for packed_sgemm
		float* pPack = packA + (p / kc) * M_align * kc;
		for(int i = 0; i < M; i += ROW_BATCH)
		{
			int k_len = kc;
			int j_len = ROW_BATCH;
			if(M - i < ROW_BATCH){
				j_len = M - i;
			}
			float* pA = A + i * lda + p;
			if(K - p < kc)
				k_len = K - p;
			//Every ROW_BATCH rows are batched together.
			for(int k = 0; k < k_len; ++k)
			{
				for(int j = 0; j < j_len; ++j)
				{
					pPack[j] = pA[j * lda];
				}
				pPack += ROW_BATCH;
				pA++;
			}
		}
	}
}

template void packed_sgemm_init<6>(int M, int K, int kc, float* packedA, float* A, int lda);

void pack_B_avx(int kc, int nc, float* packB, float* B, int ldb)
{
	const int COL_BATCH = 16;
	int nc_floor = nc - nc % COL_BATCH;
	for(int k = 0; k < kc; ++k)
	{
		float* pB = B + k * ldb;
		for(int j = 0; j < nc_floor; j+=COL_BATCH)
		{
			float* pPack = packB + (j / COL_BATCH) * kc * COL_BATCH + k * COL_BATCH;
			_mm256_store_ps(pPack, _mm256_load_ps(pB));
			_mm256_store_ps(pPack + 8, _mm256_load_ps(pB + 8));
			pB += 16;
		}
		if(nc_floor < nc)
		{
			int j = nc_floor;
			int n_len = nc - nc_floor;
			float* pPack = packB + (j / COL_BATCH) * kc * COL_BATCH + k * COL_BATCH;
			for(int i = 0; i < n_len; ++i)
			{
				pPack[i] = pB[i];
			}
		}
	}
}
	
void packed_sgemm(int M, int N, int K, float *packA, float *b, int ldb, float *c, int ldc, int nc, int kc)
{
	for(int i = 0; i < M; ++i){
		memset(c + ldc * i, 0, sizeof(float) * N);
	}

	int M_align = align_ceil(M, 6);
	int N_align = align_ceil(N, 16);

	int NBlocks = (N_align + nc - 1) / nc;
	int KBlocks = (K + kc - 1) / kc;
	
	float* packB = (float *) _mm_malloc(sizeof(float) * kc * nc, 32);
	//float* loadC = (float *) _mm_malloc(sizeof(float) * 6 * nc, 32);
	
	//Our GEMM is implemented in GEPB fashion, as the operands are row-major
	int k_len = kc;
	int n_len = nc;
	for(int kt = 0; kt < KBlocks; ++kt)
	{
		k_len = (kt == KBlocks - 1) ? (K - kt * kc) : kc;
		for(int nt = 0; nt < NBlocks; ++nt)
		{
			float loadC[6 * nc];
			float* pA = packA + kt * kc * M_align;
			float* pB = b + kt * kc * ldb + nt * nc;
			float* pC = c + nt * nc; 
			if(nt == NBlocks - 1)
				n_len = N - nt * nc;
			else
				n_len = nc;
			//I'm going to pack B in here.
			memset(packB, 0, sizeof(float) * kc * nc);
			pack_B_avx(k_len, n_len, packB, pB, N);
			compute_block(M_align, n_len, k_len, pA, packB, loadC, pC, ldc);
		}
	}
	_mm_free(packB);
}
