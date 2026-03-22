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

    cout<<"inainte de criptare " << BigInt(42)<<endl;
    BigInt criptat = engine.encrypt(BigInt(42));
    cout<<"criptat " << criptat<<endl;
    BigInt decriptat = engine.decrypt(criptat);
    cout<< "dupa decriptare " << decriptat << endl;
    cout << engine << endl;

    cout<<key.getOwner() << endl;

    cin>> engine; 
    return 0;
} 