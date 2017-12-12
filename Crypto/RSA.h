#ifndef _RSA_H_
#define _RSA_H_
#include <string>
#include "include\mpir.h"

using namespace std;

namespace rsa
{
    string encrypt(string m, mpz_t n, mpz_t e);
    string decrypt(string c, mpz_t d, mpz_t n);
}

#endif