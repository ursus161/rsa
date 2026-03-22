#include <iostream>
#include "rsakey.h"
#include "bigint.h"
#include "rsaengine.h"
using namespace std;

int main(){

    cout<<BigInt(3)+BigInt(5)<<endl;
    cout<<RSAKey::modInv(BigInt(3),BigInt(5))<<endl;
    RSAKey key(BigInt(257), BigInt(263), "Stefan");
    key.generate();
    RSAEngine engine(&key);
    engine.benchmark(3);
    
    BigInt criptat = engine.encrypt(BigInt(42));
    cout<<criptat<<endl;
    BigInt decriptat = engine.decrypt(criptat);
    cout<< decriptat << endl;
    cout << engine;
    return 0;
} 