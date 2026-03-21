#include <iostream>
#include "rsakey.h"
#include "bigint.h"
using namespace std;

int main(){

    cout<<BigInt(3)+BigInt(5)<<endl;
    cout<<RSAKey::modInv(BigInt(3),BigInt(5))<<endl;
    return 0;
} 