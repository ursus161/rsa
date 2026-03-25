#pragma once
#include <iostream>
#include "bigint.h"

class RSAKey {

//deci in primul rand vreau sa explic logica, pentru ca poate fi confuzing, 
//trb sa iau 2 numere prime, de obicei se aleg mari pt a fi mai usor de ghicit, p si q si fac n = p * q
//phi (nu o sa caut simbolul grecesc :) ) = (q-1)*(p-1) e functia lui euler
//apoi se ia un exponent e, conventia e sa fie 65537, care la prima vedere pare in jur de 2^16, si chiar este 2^16+1, la o cautare amanuntita aflam ca e al cincilea nr al lui fermat
// mai am si d care e inversul modular al lui e mod phi, deci fac un e*d%phi = 1

private:

    static const BigInt e  ; //o sa ramana mereu acelasi, e standard asa ca nu l mai recreez de fiecare data
    BigInt p,q;
    BigInt n;  
    BigInt d;
    const int bitSize;
    char *owner;
    static int keyCount; //nr de chei totale

public:

    RSAKey(const BigInt& p, const BigInt& q, const char* ownerName);
    RSAKey(const RSAKey& obj); // copy constructorul

    RSAKey& operator=(const RSAKey& obj);

    ~RSAKey();
    
    static BigInt modInv(const BigInt& a, const BigInt& m);

    void generatePrivateKey();

    friend ostream& operator<<(ostream& os, const RSAKey& key);
    friend istream& operator>>(istream& is, RSAKey& key);



    const BigInt& getN() const;
    const BigInt& getD() const;
    static const BigInt& getE(); //static pentru ca e-ul fiind unic (F4) este independent de obiect

    const char* getOwner() const;
    void setOwner(const char* newOwner);
    int getBitSize() const;
};