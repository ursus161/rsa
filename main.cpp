#include <iostream>
#include "rsakey.h"
#include "bigint.h"
#include "rsaengine.h"
#include "message.h"
using namespace std;

int main() {

//     // pointeri la obiectele active
//     RSAKey* currentKey = nullptr;
//     RSAEngine* engine = nullptr;
//     Message* msg = nullptr;

//     int choice;

//     do {
//         cout << "\n=== RSA Cryptosystem ===" << endl;
//         cout << "1. Generate RSA key" << endl;
//         cout << "2. Display key" << endl;
//         cout << "3. Change owner" << endl;
//         cout << "4. Enter message" << endl;
//         cout << "5. Encrypt message" << endl;
//         cout << "6. Decrypt message" << endl;
//         cout << "7. Display message" << endl;
//         cout << "8. Benchmark" << endl;
//         cout << "9. Toggle hex/decimal" << endl;
//         cout << "10. Exit" << endl;
//         cout << "Choice: ";
//         cin >> choice;

//         switch (choice) {

//         // GENERARE CHEIE
//         // citesc p si q in hex (sunt numere prime), plus un owner name
//         // din p si q se face: n = p*q, phi = (p-1)*(q-1), d = modInv(e, phi)
//         // engine-ul e legat de cheie, il recreez si pe el
//         case 1: {
//         if (currentKey != nullptr) {
//             char answer;
//             cout << "A key already exists for " << currentKey->getOwner() << ". Overwrite? (y/n): ";
//             cin >> answer;
//             if (answer != 'y') break;
//             delete engine;
//             delete currentKey;
//         }

//         cout << "Select key size:" << endl;
//         cout << "1. Small  (p=101, q=103)" << endl;
//         cout << "2. Medium (p=257, q=263)" << endl;
//         cout << "3. Large  (p=1009, q=1013)" << endl;
//         cout << "Choice: ";
        
//         int keyChoice;
//         cin >> keyChoice;

//         BigInt p, q;
//         switch (keyChoice) {

//             //realmente vorbind tind sa mentionez ca alegerea acestor p si q este una oribila pentru ca sunt ft close
//             //ele fiind close pot fi sparte instant folosind factorizarea fermat ( care incepe sa itereze de la a = sqrt n)
//             //alegerea acestora este in scop pur demonstrativ pentru functionalitatea aplcaitiei    
//             case 1:
//              p = BigInt(101); q = BigInt(103); break;
//             case 2:
            
//             p = BigInt(257); q = BigInt(263); break;
//             case 3:
//              p = BigInt(1009); q = BigInt(1013); break;

//             default: cout << "Invalid choice." << endl; break;
//         }

//         string owner;
//         cout << "Owner: ";
//         cin >> owner;

//         currentKey = new RSAKey(p, q, owner.c_str());
//         currentKey->generatePrivateKey();
//         engine = new RSAEngine(currentKey);

//         cout << "Key generated successfully!" << endl;
//         cout << *currentKey;
//         break;
// }

//         // AFISARE CHEIE
//         // operator<< pe RSAKey — afiseaza owner, n, e, d, bitSize
//         case 2: {
//             if (!currentKey) { cout << "No key exists. Generate one first." << endl; break; }
//             cout << *currentKey;
//             break;
//         }

//         // SCHIMBA OWNER
//         // operator>> pe RSAKey — citeste un nou owner name
//         case 3: {
//             if (!currentKey) { cout << "No key exists. Generate one first." << endl; break; }
//             cin >> *currentKey;
//             cout << "Owner changed to: " << currentKey->getOwner() << endl;
//             break;
//         }

//         // INTRODU MESAJ
//         // creez un obiect Message din textul introdus
//         // getline ca sa pot citi si spatii, cin.ignore() ca sa consum \n-ul ramas de la cin >> choice
//         case 4: {
//             if (msg != nullptr) {
//                 char answer;
//                 cout << "A message already exists: \"" << msg->getText() << "\". Overwrite? (y/n): ";
//                 cin >> answer;
//                 if (answer != 'y') break;
//                 delete msg;
//             }

//             string input;
//             cout << "Message: ";
//             cin.ignore();
//             getline(cin, input);

//             msg = new Message(input.c_str());
//             cout << "Message saved." << endl;
//             break;
//         }

//         // CRIPTARE
//         // toBlocks() transforma fiecare caracter in BigInt: 'H' -> BigInt(72), 'e' -> BigInt(101), etc imi ia codif ascii
//         // apoi fiecare bloc se cripteaza: bloc = bloc^e mod n (prin engine->encrypt care apeleaza modpow)
//         // rezultatul e un numar mare pe fiecare pozitie — textul original nu mai e vizibil (il cripteaza, that s the point)
//         case 5: {
//             if (!currentKey) { cout << "No key exists. Generate one first." << endl; break; }
//             if (!msg) { cout << "No message exists. Enter one first." << endl; break; }
//             if (msg->getIsEncrypted()) { cout << "Message is already encrypted." << endl; break; }
//             // aici zice ca e deja criptata, probabil la proiecte viitoare o sa modific pentru double encrypt cu AES
//             //sau o voi lasa doar flag de criptare RSA 

//             msg->toBlocks(); // "Hello" -> [72, 101, 108, 108, 111] de ex

//             BigInt* blocks = msg->getBlocks(); // pointer la array-ul intern
//             for (int i = 0; i < msg->getBlockCount(); i++) {
//                 blocks[i] = engine->encrypt(blocks[i]); // blocks[i] = blocks[i]^e mod n
//             }
//             msg->setEncrypted(true);

//             cout << "Message encrypted! (" << msg->getBlockCount() << " blocks)" << endl;
//             break;
//         }

//         // DECRIPTARE
//         // operatia inversa: fiecare bloc se decripteaza: bloc = bloc^d mod n
//         // fromBlocks() reconstruieste textul din blocurile decriptate: BigInt(72) -> 'H', etc
//         case 6: {
//             if (!currentKey) { cout << "No key exists. Generate one first." << endl; break; }
//             if (!msg) { cout << "No message exists. Enter one first." << endl; break; }
//             if (!msg->getIsEncrypted()) { cout << "Message is not encrypted." << endl; break; }

//             BigInt* blocks = msg->getBlocks();
//             for (int i = 0; i < msg->getBlockCount(); i++) {
//                 blocks[i] = engine->decrypt(blocks[i]); // blocks[i] = blocks[i]^d mod n
//             }
//             msg->fromBlocks(); // [72, 101, 108, 108, 111] -> "Hello"

//             cout << "Decrypted message: " << msg->getText() << endl;
//             break;
//         }

//         // AFISARE MESAJ
//         // operator<< pe Message — daca e criptat, afiseaza blocurile ca numere mari
//         // daca nu e criptat, afiseaza textul normal
//         case 7: {
//             if (!msg) { cout << "No message exists." << endl; break; }
//             cout << *msg << endl;
//             break;
//         }

//         // BENCHMARK
//         // operator>> pe RSAEngine — citeste nr de iteratii, ruleaza modpow de atatea ori
//         // afiseaza media geometrica a timpilor (prin operator<< intern)
//         case 8: {
//             if (!engine) { cout << "No key/engine exists." << endl; break; }
//             cin >> *engine;
//             break;
//         }

//         // TOGGLE HEX/DEC
//         // schimba modul de afisare pentru TOATE BigInt-urile (e static)
//         // hex: afiseaza in baza 16 (ex: 1A2B)
//         // dec: afiseaza in baza 10 (ex: 6699)
//         case 9: {
//             if (BigInt::getHexmode()) {
//                 BigInt::setDec();
//                 cout << "Display mode: decimal" << endl;
//             } else {
//                 BigInt::setHex();
//                 cout << "Display mode: hexadecimal" << endl;
//             }
//             break;
//         }

//         case 10:
//             cout << "Exiting." << endl;
//             break;

//         default:
//             cout << "Invalid option." << endl;
//             break;
//         }

//     } while (choice != 10);

//     // cleanup — eliberez tot ce am alocat cu new
//     delete currentKey;
//     delete engine;
//     delete msg;

BigInt a("DEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEF");
BigInt b("CAFEBABECAFEBABECAFEBABECAFEBABECAFEBABECAFEBABECAFEBABECAFEBABECAFEBABECAFEBABECAFEBABECAFEBABECAFEBABECAFEBABECAFEBABECAFEBABE");
    BigInt result = a * b;
    result.setHex();
    cout<<"rez este :  "<< result;
    return 0;
}