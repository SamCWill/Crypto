#include "RSA.h"
#include "Cryptomath.h"
#include "Common.h"
#include <iostream>

//c = m^e (mod n)
string rsa::encrypt(string m, mpz_t n, mpz_t e)
{
    //check input
    for(int i = 0; i < m.size(); i++)
        if(!isalpha(m[i]))
        {
            cout << "Please input letters only. Returning empty string." << endl;
            string empty;
            return empty;
        }

    //set string lowercase
    m = stringToLower(m);

    mpz_t mNum;
    mpz_init(mNum);

    //convert entire plaintext string to a number
    //Only works for m < n
    //Could also have done a couple of letters at a time
    for(int i = 0; i < m.size(); i++ )
    {
        mpz_mul_ui(mNum, mNum, 26);
        mpz_add_ui(mNum, mNum, (unsigned int)(m[i] - 'a'));
    }

    //c = m^e (mod n)
    //only works for m < n
    mpz_t* cNum = cryptomath::modular_pow(mNum, e, n);

    string c;

    //convert c to string
    while( mpz_cmp_ui(*cNum, 0) > 0 )
    {
        mpz_t mod;
        mpz_init(mod);
        //must insert at beginning of string rather than end
        //in order to mathc the order in which we removed characters
        c.insert(c.begin(), (mpz_mod_ui( mod, *cNum, 26) + 'a'));
        mpz_sub(*cNum, *cNum, mod);
        mpz_divexact_ui(*cNum, *cNum, 26);
    }

    return c;
}

//m = c^d (mod n)
string rsa::decrypt(string c, mpz_t d, mpz_t n)
{
    //check input
    for(int i = 0; i < c.size(); i++)
        if(!isalpha(c[i]))
        {
            cout << "Please input letters only. Returning empty string." << endl;
            string empty;
            return empty;
        }

    //set lower
    c = stringToLower(c);

    mpz_t cNum;
    mpz_init(cNum);

    //convert ciphertext to a number
    for(int i = 0; i < c.size(); i++ )
    {
        mpz_mul_ui(cNum, cNum, 26);
        mpz_add_ui(cNum, cNum, (unsigned int)(c[i] - 'a'));
    }

    //m = c^d (mod n)
    mpz_t* mNum = cryptomath::modular_pow(cNum, d, n);

    string m;

    //convert back to a string
    while( mpz_cmp_ui(*mNum, 0) > 0 )
    {
        mpz_t mod;
        mpz_init(mod);
        m.insert(m.begin(), (mpz_mod_ui( mod, *mNum, 26) + 'a'));
        mpz_sub(*mNum, *mNum, mod);
        mpz_divexact_ui(*mNum, *mNum, 26);
    }

    return m;
}