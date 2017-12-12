#include "Onetime.h"
#include <cmath>
#include <stdlib.h>
#include <iostream>
#include "Common.h"

//For each letter: c = m + rand() (mod m)
string onetime::encode(const string key, const string plaintext)
{
    //make sure input is valid
    for(int i = 0; i < plaintext.size(); i++)
        if(!isalpha(plaintext[i]))
        {
            cout << "Please input letters only. Returning empty string." << endl;
            string empty;
            return empty;
        }

    //set lower
    string ciphertext = stringToLower(plaintext);

    //convert keystring to a number
    unsigned int key32Bit = 0;
    for( int i = 0; i < key.size(); i++ )
        key32Bit += (key[i] - 'a') * pow(26, i);
    //seed rng with key
    srand(key32Bit);

    //just add em up
    for( int i = 0; i < plaintext.size(); i++ )
        ciphertext[i] = ((ciphertext[i] - 'a' + (rand() % 26)) % 26) + 'a';

    return ciphertext;
}

//now we're subtracting the same numbers
//so long as the seed is the same, the entire string
//of pseudorandom numbers will also be the same,
//and the reversal of the one-time pad will be easy
string onetime::decode(const string key, const string ciphertext)
{
    //check inputs
    for(int i = 0; i < ciphertext.size(); i++)
        if(!isalpha(ciphertext[i]))
        {
            cout << "Please input letters only. Returning empty string." << endl;
            string empty;
            return empty;
        }

    //set lower
    string plaintext = stringToLower(ciphertext);

    //convert key to number
    unsigned int key32Bit = 0;
    for( int i = 0; i < key.size(); i++ )
        key32Bit += (key[i] - 'a') * pow(26, i);
    srand(key32Bit);

    //undo encryption
    for( int i = 0; i < ciphertext.size(); i++ )
        plaintext[i] = ((plaintext[i] - 'a' - (rand() % 26) + 26) % 26) + 'a';

    return plaintext;
}