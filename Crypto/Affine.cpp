#include "Affine.h"
#include "Common.h"
#include "Cryptomath.h"
#include <cctype>
#include <iostream>

//Encodes a message wit the equation ax + b (mod m )
string affine::encode(const unsigned int a, const unsigned int b, const string plaintext)
{
    //make sure there are only legal characters in input
    for(int i = 0; i < plaintext.size(); i++)
        if(!isalpha(plaintext[i]))
        {
            cout << "Please input letters only. Returning empty string." << endl;
            string empty;
            return empty;
        }

    //set input t0 lowercase
    string ciphertext = stringToLower(plaintext);

    //for each character in input
    for(int i = 0; i < ciphertext.size(); i++)
        //ax + b (mod m )
        ciphertext[i] = 'a' + ((a * (ciphertext[i] - 'a') + b) % m);

    return ciphertext;
}

//decode with a^-1 * (x - b) (mod m)
//a^-1 is mod inverse of a (mod m)
string affine::decode(const unsigned int a, const unsigned int b, const string ciphertext)
{
    //check input
    for(int i = 0; i < ciphertext.size(); i++)
        if(!isalpha(ciphertext[i]))
        {
            cout << "Please input letters only. Returning empty string." << endl;
            string empty;
            return empty;
        }

    //set lowercase
    string plaintext = stringToLower(ciphertext);

    //for each character in ciphertext
    for(int i = 0; i < plaintext.size(); i++)
        //a^-1 * (x - b) (mod m)
        plaintext[i] = 'a' + ((cryptomath::findModInverse(a, m) * (plaintext[i] - 'a' - b + 26) % m) % m);

    return plaintext;
}

//Given known plaintext, returns a and b
void affine::knownPlaintextAttack(unsigned int* a, unsigned int* b, const string ciphertext, const string plaintext)
{
    string c = stringToLower(ciphertext);
    string p = stringToLower(plaintext);
    //gets first character translation
    char p1 = p[0] - 'a';
    char c1 = c[0] - 'a';
    char p2;
    char c2;
    int i;

    //gets one different character translation
    for( i = 1; p[i] == p[0]; i++ );

    p2 = p[i] - 'a';
    c2 = c[i] - 'a';

    //make sure p1 is bigger
    if( p1 < p2 )
    {
        int t = p1;
        p1 = p2;
        p2 = t;
        t = c1;
        c1 = c2;
        c2 = t;
    }
    //if fraction can be reduced, reduce it
    int v1 = (c1 - c2 + 26) % 26;
    int v2 = p1 - p2;
    int g = gcd(v1 , v2);
    v1 = v1 / g;
    v2 = v2 / g;
    //find modular inverse of denominator mod 26 and mutliply it with denominator
    *a = v1 * cryptomath::findModInverse(v2, 26) % 26;
    //having found a, deriving b is simple
    *b = (c1 - p1 * *a + 26 * 26) % 26;
}