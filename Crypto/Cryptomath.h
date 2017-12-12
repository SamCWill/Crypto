#ifndef _CRYPTOMATH_H_
#define _CRYPTOMATH_H_
#include <string>
#include "include\mpir.h"

using namespace std;

namespace cryptomath
{
    const int m = 26; //alphabet size
    void randinit();
    mpz_t* gcd( mpz_t a, mpz_t b );
    unsigned int extendedgcd( const unsigned int a, const unsigned int b, unsigned int* x, unsigned int* y );
    unsigned int findModInverse( const unsigned int a, const unsigned int n );
    mpz_t* modular_pow(mpz_t base, mpz_t exponent, mpz_t modulus);
    bool is_prime( mpz_t n );
    mpz_t* random_prime( unsigned int b );
    void factor( mpz_t n, char m, mpz_t* p, mpz_t* q );
    mpz_t* test( mpz_t a, mpz_t b);
    void oneMore();
}

#endif