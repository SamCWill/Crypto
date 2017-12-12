#include "Affine.h"
#include "Vigenere.h"
#include "Onetime.h"
#include "DES.h"
#include "Cryptomath.h"
#include <iostream>
#include "mpir.h"
#include "RSA.h"

using namespace std;

int main()
{
    cryptomath::randinit();

    string ciphertext = affine::encode(15, 5, "oksurewhynot");
    string plaintext = affine::decode(15, 5, ciphertext);

    unsigned int a, b;

    affine::knownPlaintextAttack(&a, &b, ciphertext, plaintext);

    cout << ciphertext << " " << plaintext << " " << a << " " << b << endl;

    ciphertext = vigenere::encode("keysa", "oksurewhynotthisneedstobealotlongerforthethingtoworkamazingohyeahwoweemamaitlookslikemakingitthatlastbitlongeractuallybroughtthekeylengthfartherofftargetithinkilljustkeepmakingthislongerandlongertoseewhathappens");
    plaintext = vigenere::decode("keysa", ciphertext);

    string key = vigenere::attack(ciphertext);

    cout << ciphertext << " " << plaintext << " " << key << endl;

    ciphertext.clear();
    plaintext.clear();

    ciphertext = onetime::encode("key", "ohboythisisgettingwackyandwild");
    plaintext = onetime::decode("key", ciphertext);

    cout << ciphertext << " " << plaintext << endl;

    ciphertext.clear();
    plaintext.clear();

    int output = des::encode(153, 1830);
    int input = des::decode(153, output);

    mpz_t bigPrime;
    mpz_init_set_ui(bigPrime, 15485993);
    //bool test = cryptomath::is_prime(bigPrime);
    //mpz_t* randPrime = cryptomath::random_prime(20);
    mpz_t p, q;
    mpz_init(p);
    mpz_init(q);
    mpz_t n;
    mpz_init_set_ui(n, 7927);
    mpz_mul_ui(n, n, 8171);
    cryptomath::factor(n, 1, &p, &q);

    mpz_t e;
    mpz_init_set_ui(e, 17);

    mpz_t d;
    mpz_init_set_ui(d, 53327993);

    ciphertext = rsa::encrypt("ireal", n, e);
    plaintext = rsa::decrypt(ciphertext, d, n);

    return 0;
}