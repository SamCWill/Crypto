#ifndef _ONETIME_H_
#define _ONETIME_H_
#include <string>

using namespace std;

namespace onetime
{
    const int m = 26; //alphabet size
    string encode(const string key, const string plaintext);
    string decode(const string key, const string ciphertext);
}

#endif