#include "Vigenere.h"
#include "Common.h"
#include <iostream>
#include <cmath>

//encodes by looping through key, adding key character values to plaintext
string vigenere::encode(const string key, const string plaintext)
{
    //check input
    for(int i = 0; i < plaintext.size(); i++)
        if(!isalpha(plaintext[i]))
        {
            cout << "Please input letters only. Returning empty string." << endl;
            string empty;
            return empty;
        }

    //set lowercase
    string ciphertext = stringToLower(plaintext);

    //for every character in the message
    for(int i = 0; i < ciphertext.size(); i++)
        //cycle through key adding character values
        ciphertext[i] = (((ciphertext[i] - 'a') + (key[i % key.size()] - 'a')) % m) + 'a';

    return ciphertext;
}

//same as encode except we're subtracting key values instead of adding them
string vigenere::decode(const string key, const string ciphertext)
{
    //check input
    for(int i = 0; i < ciphertext.size(); i++)
        if(!isalpha(ciphertext[i]))
        {
            cout << "Please input letters only. Returning empty string." << endl;
            string empty;
            return empty;
        }

    //set lower
    string plaintext = stringToLower(ciphertext);

    //for every character in ciphertext
    for(int i = 0; i < plaintext.size(); i++)
        //loop through key subtracting character values
        plaintext[i] = (((plaintext[i] - 'a') - (key[i % key.size()] - 'a') + m) % m) + 'a';

    return plaintext;
}

//attack with lost of frequency analysis
//view comments within body for more details
string vigenere::attack(const string ciphertext)
{
    //coincidence index for every key length
    //we can viably check
    //When they start getting to the point where
    //we only have one character in some columns...
    //this method starts to break down a bit
    float* coincidence = new float[ciphertext.size() / 2];

    //for each viable key length
    for( int i = 1; i < ciphertext.size() / 2; i++ )
    {
        //create 1 column for each character in the key
        string* columns = new string[i];
        //populate the columns by inserting one character into
        //each and then looping back around and repeating
        //until every character in the ciphertext has been
        //placed into a column
        for( int j = 0; j < ciphertext.size(); j++ )
            columns[j % i] += ciphertext[j];

        //the coincidence indices of each column added together
        float columnSum = 0;

        //Get the coincidence inex of each column one at a time and add them up
        for( int j = 0; j < i; j++ )
        {
            int* freq = frequency(columns[j]);
            int sum = 0;
            for( int k = 0; k < 26; k++ )
                sum += freq[k] * (freq[k] - 1);

            columnSum += (float)(sum * 26.0) / (float)(columns[j].size() * (columns[j].size() - 1));
        }
        //divide by the number of columns for an avere for that key length
        coincidence[i] = columnSum / (float)i;
    }

    //keeps track of bestCoincidence so far
    float bestCoincidence = 0;
    int probableKeyLength = INT_MAX;

    //walks through each key length
    for( int i = 1; i < ciphertext.size() / 2 && !bestCoincidence; i++ )
    {
        float sum = 0;
        int divisor = 0;
        //checks all key lengths multiple of target as well
        //averages them together
        //if 2 is correct, then 4, 6, 8, etc will all also have high scores
        for( int j = i; j < ciphertext.size() / 2; j += i )
        {
            sum += coincidence[j];
            divisor++;
        }

        //if the score is high enough
        //(int this case, if it's above this arbitrarily chosen threshold)
        //we're done!
        if( sum/divisor > 1.5 )
        {
            bestCoincidence = sum/divisor;
            probableKeyLength = i;
        }
    }

    //finding the key length that is
    //Now we need to recreate the columns for that key length
    string* columns = new string[probableKeyLength];
    for( int j = 0; j < ciphertext.size(); j++ )
        columns[j % probableKeyLength] += ciphertext[j];

    //and we'll also need the frequencies of every letter in the english alphabet
    //which I was able to snag off of Wikipedia
    float englishFrequencies[26] = {
        8.167,
        1.492,
        2.782,
        4.253,
        12.702,
        2.228,
        2.015,
        6.094,
        6.966,
        0.153,
        0.772,
        4.025,
        2.406,
        6.749,
        7.507,
        1.929,
        0.095,
        5.987,
        6.327,
        9.056,
        2.758,
        0.978,
        2.360,
        0.150,
        1.974,
        0.074    
    };

    string key;

    //for each column
    for( int i = 0; i < probableKeyLength; i++ )
    {
        float bestScore = 0;
        int bestLetter = 0;
        //we're going to try each 'rotation' of the English alphabet
        //and we're going to multiply all of the frequencies in that
        //rotation with the frequencies of our column
        for( int j = 0; j < 26; j++ )
        {
            int* freq = frequency(columns[i]);
            float sum = 0;
            for( int k = 0; k < 26; k++ )
                sum += freq[(k + j) % 26] * englishFrequencies[k];

            //the rotation with the best score should point straight
            //to the key character value for that column
            if( sum > bestScore )
            {
                bestScore = sum;
                bestLetter = j;
            }
        }
        //put them all together to get the whole key!
        key += 'a' + bestLetter;
    }

    return key;
}