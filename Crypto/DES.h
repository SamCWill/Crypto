#ifndef _DES_H_
#define _DES_H_
#include <string>

using namespace std;

namespace des
{
    const int rounds = 4;
    int encode(const int key, const int plaintext);
    int decode(const int key, const int ciphertext);
    int attackThreeRounds( const int L1R1 );
}

#endif