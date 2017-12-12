#ifndef _AFFINE_H_
#define _AFFINE_H_
#include <string>

using namespace std;

namespace affine
{
    const int m = 26; //alphabet size
    string encode(const unsigned int a, const unsigned int b, const string plaintext);
    string decode(const unsigned int a, const unsigned int b, const string ciphertext);
    void knownPlaintextAttack(unsigned int* a, unsigned int* b, const string ciphertext, const string plaintext);
}

#endif