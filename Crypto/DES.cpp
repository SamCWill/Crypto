#include "DES.h"

//S boxes
int S1[2][8] = { { 5, 2, 1, 6, 3, 4, 7, 0 }, { 1, 4, 6, 2, 0, 7, 5, 3 } };
int S2[2][8] = { { 4, 0, 6, 5, 7, 1, 3, 2 }, { 5, 3, 0, 7, 6, 2, 1 ,4 } };

//expansion as described in the book
//& 0x-------- are used to single out individual bits
int expand(int r)
{
    return (r & 0x00000003) ^ ((r >> 1) & 0x00000004) ^ ((r << 1) & 0x00000018) ^ ((r << 3) & 0x00000020) ^ ((r << 2) & 0x000000C0);
}

//des function expands, xors with key, and then accesses s-boxes and returns 6 new bits
int desf( int r, int k )
{
    r = expand(r);
    r ^= k;
    //S1[bit 1][bits 2-4]
    //S2[bit 5][bits 6-8]
    //1 being most significant in this specific instance
    return S1[r >> 7][(r >> 4) & 0x00000007] << 3 ^ S2[(r >> 3) & 0x00000001][r & 0x00000007];
}

//finds key for a specific round
int ki( int k, int i )
{
    //K = 123456789
    //ki( K, 1 ) = 23456789
    //ki( K, 2 ) = 34567891
    //etc
    return ((k << (i - 1)) ^ (k >> (10 - i))) & 0x000000FF;
}

//encodes one 12 bit block over 4 rounds
int des::encode(const int key, const int plaintext)
{
    //seperates into left and right 6 bits
    //in the case that ints with too many active bits are passed in
    //only the number of usable bits are isolated
    int Lprev = (plaintext >> 6) & 0x0000003F;
    int Rprev = plaintext & 0x0000003F;
    int K = key & 0x000001FF;

    //perform 4 rounds
    for( int i = 0; i < des::rounds; i++ )
    {
        //set l = r
        int Lnext = Rprev;
        //set r = l ^ f(r, ki)
        int Rnext = Lprev ^ desf(Rprev, ki(K, i + 1));
        Lprev = Lnext;
        Rprev = Rnext;
    }
    //put the two halves together and return em
    return Rprev ^ (Lprev << 6);
}

//decodes one 12 bit block over 4 rounds
int des::decode(const int key, const int ciphertext)
{
    //isolates and splits bits
    int Lnext = (ciphertext >> 6) & 0x0000003F;
    int Rnext = ciphertext & 0x0000003F;
    int K = key & 0x000001FF;

    //4 rounds
    //same as encrypt bit reversed
    for( int i = 0; i < des::rounds; i++ )
    {
        //r = l;
        int Rprev = Lnext;
        //l = r ^ f(l, ki)
        int Lprev = Rnext ^ desf(Lnext, ki(K, des::rounds - i));
        Lnext = Lprev;
        Rnext = Rprev;
    }

    //pack em up and send em off
    return Rnext ^ (Lnext << 6);
}

//ran out of time to implement this
//honestly, I let this project get the better of me
//I underestimated it and in hindsight wish I had spent a lot
//more time working on it earlier on
int des::attackThreeRounds( const int L1R1 )
{
    return 0;
}