#ifndef __RAY_MATH_TOOLKIT_H
#define __RAY_MATH_TOOLKIT_H

#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <emmintrin.h>

static inline
void normalize(double *v)
{
    double d = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    assert(d != 0.0 && "Error calculating normal");

    v[0] /= d;
    v[1] /= d;
    v[2] /= d;
}

static inline
double length(const double *v)
{
    return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

static inline
void add_vector(const double *a, const double *b, double *out)
{
    __m128d lvfA = _mm_set_pd( a[1], a[0]);
    __m128d lvfB = _mm_set_pd( b[1], b[0]);
    __m128d lvfA_B = _mm_add_pd( lvfA, lvfB );
    _mm_store_pd( out, lvfA_B );
    out[2] = a[2] + b[2];
}

static inline
void subtract_vector(const double *a, const double *b, double *out)
{
    out[0] = a[0] - b[0];
    out[1] = a[1] - b[1]; 
    out[2] = a[2] - b[2];
}

static inline
void multiply_vectors(const double *a, const double *b, double *out)
{
    __m128d lvfA = _mm_set_pd( a[1], a[0]);
    __m128d lvfB = _mm_set_pd( b[1], b[0]);
    __m128d lvfA_B = _mm_mul_pd( lvfA, lvfB );
    _mm_store_pd( out, lvfA_B );
    out[2] = a[2] * b[2];
}

static inline
void multiply_vector(const double *a, double b, double *out)
{
    out[0] = a[0] * b;
    out[1] = a[1] * b;
    out[2] = a[2] * b;
}

static inline
void cross_product(const double *v1, const double *v2, double *out)
{
    __m128d lvfA = _mm_set_pd( v1[2], v1[1]);
    __m128d lvfB = _mm_set_pd( v2[0], v2[2]);
    __m128d lvfC = _mm_set_pd( v1[0], v1[2]);
    __m128d lvfD = _mm_set_pd( v2[2], v2[1]);

    __m128d lvfA_mult_B = _mm_mul_pd( lvfA, lvfB );
    __m128d lvfC_mult_D = _mm_mul_pd( lvfC, lvfD );
    __m128d sub = _mm_sub_pd( lvfA_mult_B, lvfC_mult_D);
    _mm_store_pd( out, sub);

    out[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

static inline
double dot_product(const double *v1, const double *v2)
{
    double dp = 0.0;
    double out[2] = {0.0, 0.0};
    __m128d lvfA = _mm_set_pd( v1[1], v1[0]);
    __m128d lvfB = _mm_set_pd( v2[1], v2[0]);
    __m128d lvfA_mult_B = _mm_mul_pd( lvfA, lvfB );
    _mm_store_pd( out, lvfA_mult_B );
	    
    dp = out[0] + out[1];
    dp += v1[2] * v2[2];
    return dp;
}

static inline
void scalar_triple_product(const double *u, const double *v, const double *w,
                           double *out)
{
    cross_product(v, w, out);
    multiply_vectors(u, out, out);
}

static inline
double scalar_triple(const double *u, const double *v, const double *w)
{
    double tmp[3];
    cross_product(w, u, tmp);
    return dot_product(v, tmp);
}

#endif
