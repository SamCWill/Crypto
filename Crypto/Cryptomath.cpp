#include "Cryptomath.h"
#include "stdlib.h"
#include <cmath>

//state used for generation of massive random numbers
gmp_randstate_t state;

//initializes the random state used by prime functions down below
void cryptomath::randinit()
{
    gmp_randinit_default(state);
}

//Find greatest common divisor of numbers of any size
mpz_t* cryptomath::gcd( mpz_t a, mpz_t b )
{
    //Copy a and b so as not to modify them
    mpz_t a0, b0;
    mpz_init_set(a0, a);
    mpz_init_set(b0, b);

    //while b != 0
    while( mpz_cmp_ui(b0, 0) )
    {
        //b = a mod b
        //a = b
        mpz_t t;
        mpz_init_set(t, b0);
        mpz_mod(b0, a0, b0); 
        mpz_set(a0, t); 
    }
    mpz_t* ret = (mpz_t*)malloc(sizeof(mpz_t));
    mpz_init_set(*ret, a0);
    return ret;
}

//finds gcd and x and y for Diophantine equations: ax + by = gcd
unsigned int cryptomath::extendedgcd( const unsigned int a, const unsigned int b, unsigned int* x, unsigned int* y )
{
    if( a == 0 )
    {
        *x = 0, *y = 1;
        return b;
    }
 
    unsigned int x1, y1;
    //recurse
    unsigned int gcd = cryptomath::extendedgcd(b%a, a, &x1, &y1);
 
    *x = y1 - (b/a) * x1;
    *y = x1;
 
    return gcd;
}

//find modular inverse of a (mod n)
//In other words, for a*b = 1 (mod n), this equation finds b
unsigned int cryptomath::findModInverse( const unsigned int a, const unsigned int n )
{
    unsigned int t = 0;     
    unsigned int newt = 1;    
    unsigned int r = n;     
    unsigned int newr = a;    
    while(newr != 0)
    {
        unsigned int quotient = r / newr;
        unsigned int tempt = t;
        t = newt;
        newt = tempt - quotient * newt;
        unsigned int tempr = r;
        r = newr;
        newr = tempr - quotient * newr;
    }
    //failure condition. No inverse. a and n not coprime.
    if(r > 1)
        return 0;
    if(t < 0) 
        t = t + n;
    return t;
}

//Find base^exponent (mod modulus)
//Works for very large numbers
mpz_t* cryptomath::modular_pow(mpz_t base, mpz_t exponent, mpz_t modulus)
{
    mpz_t* ret = (mpz_t*)malloc(sizeof(mpz_t));
    mpz_init_set_ui(*ret, 1);
    if( !mpz_cmp_ui(modulus, 1) )
    {
        mpz_set_ui(*ret, 0);
        return ret;
    }
    mpz_t i;
    //performs multiplications of base one a time, taking mod between each multiplication
    //this really saves on space
    for( mpz_init_set_ui(i, 0); mpz_cmp(i, exponent) < 0; mpz_add_ui(i, i, 1) )
    {
        mpz_mul(*ret, *ret, base);
        mpz_mod(*ret, *ret, modulus);
    }
    return ret;
}

//returns true on probably primes
//returns false on definitely non-prime
bool cryptomath::is_prime(mpz_t n)
{
    mpz_t a;
    mpz_init(a);
    mpz_t t;
    mpz_init(t);
    mpz_sub_ui(t, n, 2);
    //generates random number: a
    mpz_urandomm(a, state, t);
    mpz_add_ui(a, a, 1);

    mpz_sub_ui(t, n, 1);
    //gets a^(n-1) (mod n)
    mpz_t* modPow = modular_pow(a, t, n);

    //if not 1, not prime
    if( mpz_cmp_si(*modPow, 1) )
    {
        free(modPow);
        return false;
    }
    //else, probably prime
    else
    {
        free(modPow);
        return true;
    }
}

//generates a random probably prime in any range 2^b - 2^(b+1)
mpz_t* cryptomath::random_prime( unsigned int b )
{
    mpz_t* candidate = (mpz_t*)malloc(sizeof(mpz_t));;
    mpz_init(*candidate);
    //generate random number 0 - 2^b
    mpz_urandomb(*candidate, state, b);
    //add 2^b to bring in range
    mpz_add_ui(*candidate, *candidate, pow(2, b));
    //while it's not random, keep generating
    while( !cryptomath::is_prime(*candidate) )
    {
        mpz_urandomb(*candidate, state, b);
        mpz_add_ui(*candidate, *candidate, pow(2, b));
    }
    //return first one found that passes test
    return candidate;
}

//returns true of n / sqtr(n) = srt(n)
bool isPerfectSquare( mpz_t n )
{
    mpz_t root;
    mpz_init(root);
    mpz_root(root, n, 2);
    mpz_mul(root, root, root);
    return !mpz_cmp(root, n);
}

//finds the factorial of a number
//ex: 6! = 6*5*4*3*2 = 720
unsigned int factorial( unsigned int n )
{
    if( n == 0 )
        return 1;
    return n * factorial(n - 1);
}

//For p, q primes, n = pq, finds p and q
//Method used for factorization decided by m
void cryptomath::factor( mpz_t n, char m, mpz_t* p, mpz_t* q )
{
    //fermat's method
    if( m == 0 )
        //only tries 0 < i < 10000
        for(int i = 1; i < 10000; i++ )
        {
            mpz_t it;
            mpz_init_set_si(it, i);
            mpz_t square;
            //square = i^2
            mpz_init_set_si(square, i*i);
            mpz_t sum;
            mpz_init(sum);
            //sum = n + i^2
            mpz_add(sum, n, square);
            //if the sum is a perfect square, we've found our pa nd q and we can stop
            if( isPerfectSquare(sum) )
            {
                mpz_t root;
                mpz_init(root);
                mpz_root(root, sum, 2);
                //p = sqrt(sum) + i
                mpz_add(*p, root, it);
                //q = sqrt(sum) - i
                mpz_sub(*q, root, it);
                return;
            }
        }
    //Pollard p-1
    else if( m == 1 )
    {
        //a = 2 is integer book uses
        //and integer > 1 is apparently fine though
        unsigned int a = 2;
        //chose B = 1000 more or less as a guess
        //too much more than that starts to take a while
        unsigned int B = 1000;
        mpz_t b;
        mpz_init_set_ui(b, a);
        mpz_t i;
        //take a^(B!) (mod n) one exponent at a time
        for( mpz_init_set_ui(i, 2); mpz_cmp_ui(i, B) <= 0; mpz_add_ui(i, i, 1) )
        {
            mpz_t* newb = modular_pow(b, i, n);
            mpz_set(b, *newb);
            free(newb);
        }
        mpz_t t;
        mpz_init(t);
        mpz_sub_ui(t, b, 1);
        // d = gcd( b - 1, n )
        mpz_t* d = cryptomath::gcd(t, n);
        //if 1 < d < n
        if( mpz_cmp_ui(*d, 1) > 0 && mpz_cmp(*d, n) < 0 )
        {
            //we got ourselves a factor
            mpz_set(*p, *d);
            //which we can use to derive another
            mpz_divexact(*q, n, *p);
        }
        free(d);
    }
}