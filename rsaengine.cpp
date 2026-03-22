
#include "rsaengine.h"
#include <chrono>
#include <cmath>


int RSAEngine::engineCount = 0;


RSAEngine::RSAEngine(RSAKey* key) {
    currentKey = key;
    lastEncryptTime = 0;
    lastDecryptTime = 0;
    benchmarkResults = nullptr;
    benchmarkCount = 0;
    engineCount++;
}

BigInt RSAEngine::encrypt(const BigInt& mesaj) {
    auto start = chrono::high_resolution_clock::now();
    BigInt result = mesaj.modpow(RSAKey::getE(), currentKey->getN()); //mesaj ^ e % n
    auto end = chrono::high_resolution_clock::now();
    lastEncryptTime = chrono::duration_cast<chrono::microseconds>(end - start).count();
    return result;
}

BigInt RSAEngine::decrypt(const BigInt& criptat) {
    auto start = chrono::high_resolution_clock::now();
    BigInt result = criptat.modpow(currentKey->getD(), currentKey->getN()); //mesaj(acum criptat) ^ d % n
    auto end = chrono::high_resolution_clock::now();
    lastDecryptTime = chrono::duration_cast<chrono::microseconds>(end - start).count();
    return result;
}


inline double RSAEngine::getLastEncryptTime() const {

    return this->lastEncryptTime;
}

inline double RSAEngine::getLastDecryptTime() const {

    return this->lastDecryptTime;
}



RSAEngine::RSAEngine(const RSAEngine& other) {


    currentKey = other.currentKey;
    lastEncryptTime = other.lastEncryptTime;
    lastDecryptTime = other.lastDecryptTime;
    benchmarkCount = other.benchmarkCount;

    if (benchmarkCount > 0) {
        benchmarkResults = new float[benchmarkCount];
        memcpy(benchmarkResults, other.benchmarkResults, benchmarkCount * sizeof(float));
    } else {
        benchmarkResults = nullptr;
    }
    engineCount++;
}


RSAEngine& RSAEngine::operator=(const RSAEngine& other) {
    if (this == &other) 
        return *this;


    currentKey = other.currentKey;
    lastEncryptTime = other.lastEncryptTime;
    lastDecryptTime = other.lastDecryptTime;
    benchmarkCount = other.benchmarkCount;

    delete[] benchmarkResults; // eliberez mem, adica ce era inainte pe *this inainte sa aloc


    if (benchmarkCount > 0) {
        benchmarkResults = new float[benchmarkCount]; // aici si aloc 
        memcpy(benchmarkResults, other.benchmarkResults, benchmarkCount * sizeof(float));
    } else {
        benchmarkResults = nullptr;
    }
    return *this;
}

void RSAEngine::benchmark(int iteratii) {
    if (benchmarkResults) delete[] benchmarkResults; //sterg rez vechi
    benchmarkCount = iteratii;
    benchmarkResults = new float[iteratii];
    
    BigInt testMsg(42); //am ales un numar random pe care sa-l testez, pot sa pun orice vreau, 99, 67 etc

    for (int i = 0; i < iteratii; i++) {
        auto start = chrono::high_resolution_clock::now();
        volatile BigInt result = testMsg.modpow(RSAKey::getE(), currentKey->getN()); /
        //asta nu a fost acoperita la lab si ar fi bine sa o mentionez
        //cand rulam teste pentru cod de asamblare, aveam probleme cu g++ care imi eficientiza codul cand voiam sa rulez benchmark uri, fapt care nu se observa decat in objdump uri
        //atunci, pe langa alte lucruri am folosit keyword ul volatile care previne compilatorul sa faca optimizari, forteaza sa faca calculul
        auto end = chrono::high_resolution_clock::now();
        benchmarkResults[i] = chrono::duration_cast<chrono::microseconds>(end - start).count();
    }
}

ostream& operator<<(ostream& os, const RSAEngine& eng){


    
    os<< "RSAEngine # " << eng.engineCount<<endl;
    os<< "Last encrypt time: " << eng.getLastEncryptTime() <<endl;
    os << "Last decrypt time: " << eng.getLastDecryptTime() << endl;
    if (eng.benchmarkCount > 0) //adica daca s a dat vreun benchmark pana acum
    {
        os<< "Benchmark results: ( " << eng.benchmarkCount << " runs ): " << endl;

        float total = 0;
        for (int i = 0 ; i< eng.benchmarkCount; i++)
            total += log(eng.benchmarkResults[i]);
        
        os << "The geometric mean of the benchmark results is : " << fixed << setprecision(2) <<exp(total / eng.benchmarkCount )<< endl; 
    }


    return os;
}



istream& operator>>(istream& is, RSAEngine& eng){ // din nou, tipul de retur e referinta ptr ca streamurile nu se pot copia ( nu au copy constr ) 
    
        int p, q;
        string owner;

        cout << "p: ";
        is >> p;
        cout << "q: ";
        is >> q;

        cout << "Owner: ";
        is >> owner;

        RSAKey* newKey = new RSAKey(BigInt(p), BigInt(q), owner.c_str());
        newKey->generate();

        eng.currentKey = newKey; //resetez cheia curenta
        return is; 
}

RSAEngine::~RSAEngine() {
    if (benchmarkResults) delete[] benchmarkResults;
    
}

