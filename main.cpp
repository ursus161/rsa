#include <iostream>
#include "rsakey.h"
#include "bigint.h"
#include "rsaengine.h"
using namespace std;

int main(){

    // cout<<BigInt(3)+BigInt(5)<<endl;

    // cout<<RSAKey::modInv(BigInt(3),BigInt(5))<<endl;

    // RSAKey key(BigInt(257), BigInt(263), "Stefan"); cout<<key<<endl;
    // key.generatePrivateKey(); //genereaza cheia privata
    // RSAEngine engine(&key);
    // engine.benchmark(3);

    // cout<<"inainte de criptare " << BigInt(42)<<endl;
    // BigInt criptat = engine.encrypt(BigInt(42));
    // cout<<"criptat " << criptat<<endl;
    // BigInt decriptat = engine.decrypt(criptat);
    // cout<< "dupa decriptare " << decriptat << endl;
    // cout << engine << endl;

    // cout<<key.getOwner() << endl;

    // cin>> engine; 

    // cout<<"key modpow " << key.modInv(BigInt(5), BigInt(12)) << endl;

    // cout<<" cheia efectiva" << endl<<key;

    // RSAKey key2(BigInt(11), BigInt(17), "Yolanda");
    // key2.generatePrivateKey();
    // RSAEngine engine2(&key2);

    // cout<<endl<< key2;
    // cout<<endl<<key2.getOwner();


    // cout<<endl<<"marimea rsaengine este" << sizeof(RSAEngine)<< " "<< sizeof(engine);
    
    uint8_t choice;

    cout<<BigInt(0);

    do { 
        cout << "\n=== RSA Cryptosystem ===" << endl;
        cout << "1. Genereaza cheie RSA" << endl;
        cout << "2. Afiseaza cheie" << endl;
        cout << "3. Cripteaza mesaj" << endl;
        cout << "4. Decripteaza mesaj" << endl;
        cout << "5. Benchmark" << endl;
        cout << "6. Schimba owner" << endl;
        cout << "7. Toggle hex/decimal" << endl;
        cout << "8. Afiseaza mesaj" << endl;
        cout << "9. Exit" << endl;
        cout << "Alegere: ";

        cin >> choice;


        switch (choice)
        {
        case 1:
            /* code */
            break;
        
        default:
            break;
        }


    } while(choice != 9 );




































    return 0;
} 