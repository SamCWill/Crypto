#ifndef _VIGENERE_H_
#define _VIGENERE_H_
#include <string>

using namespace std;

namespace vigenere
{
    const int m = 26; //alphabet size
    string encode(const string key, const string plaintext);
    string decode(const string key, const string ciphertext);
    string attack(const string ciphertext);
}

#endif