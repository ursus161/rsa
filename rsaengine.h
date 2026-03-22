#pragma once
#include <iostream>
#include "rsakey.h"


using namespace std;

class RSAEngine     {
    private:

        RSAKey* currentKey;
        double lastEncryptTime;
        double lastDecryptTime;
        float* benchmarkResults;
        int benchmarkCount;
        static int engineCount;



    public:

         RSAEngine(RSAKey* key);
        RSAEngine(const RSAEngine& other);
        RSAEngine& operator=(const RSAEngine& other);


        BigInt encrypt(const BigInt& mesaj);
        BigInt decrypt(const BigInt& criptat);
        void benchmark(int iterations);

        double getLastEncryptTime() const;
        double getLastDecryptTime() const;

        friend ostream& operator<<(ostream& os, const RSAEngine& eng);
    friend istream& operator>>(istream& is, RSAEngine& eng);

       
    ~RSAEngine();
};

