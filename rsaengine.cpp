

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


double RSAEngine::getLastEncryptTime() const {

    return this->lastEncryptTime;
}

double RSAEngine::getLastDecryptTime() const {

    return this->lastDecryptTime;
}

RSAEngine& RSAEngine::operator=(RSAEngine obj) const{


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

RSAEngine::~RSAEngine() {
    if (benchmarkResults) delete[] benchmarkResults;
    
}

