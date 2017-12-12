#include "Common.h"

//returns a copy of input string where every uppercase character is made lowercase
string stringToLower(string input)
{
    string output = input;
    for(int i = 0; i < output.size(); i++)
        output[i] = tolower(output[i]);
    return output;
}

//counts up the occurences of each character in a string
int* frequency(string text)
{
    int* frequency = new int[26];
    for( int i = 0; i < 26; i++ )
        frequency[i] = 0;

    text = stringToLower(text);

    for( int i = 0; i < text.size(); i++ )
        frequency[text[i] - 'a']++;

    return frequency;
}

//Finds greatest common divisor of small numbers
//Only used by affine cipher
int gcd(char a, char b)
{
    while( b != 0 )
    {
       int t = b; 
       b = a % b; 
       a = t; 
    }
    return a;
}