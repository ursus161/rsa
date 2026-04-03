#include "rsakey.h"

const BigInt RSAKey::e = BigInt(65537);
int RSAKey::keyCount = 0;

RSAKey::RSAKey(const BigInt& p, const BigInt& q, const char* ownerName)  : bitSize(p.bitLength() + q.bitLength()) {

    this->p = p;
    this->q = q;
    n = p * q;
    
    owner = new char[strlen(ownerName) + 1];
    strcpy(owner, ownerName);
    
    keyCount++;
}

RSAKey::RSAKey(const RSAKey& obj) : bitSize(obj.bitSize) { // copy constructorul

    this->p = obj.p;
    this->q = obj.q;
    this->n = obj.n;
    this->d = obj.d;

    owner = new char[strlen(obj.owner) + 1];
    strcpy(owner, obj.owner);

    keyCount++;
}

RSAKey& RSAKey::operator=(const RSAKey& obj) {

    if (this == &obj) return *this;

    this->p = obj.p;
    this->q = obj.q;

    this->n = obj.n;
    this->d = obj.d;    
    this->bitSize  = obj.bitSize;
    delete[] owner; // obiectul daca nu l eliberez acm mi s ar elibera memoria de abia la final de lifetime 

    owner = new char[strlen(obj.owner) + 1];
    strcpy(owner, obj.owner);
    
    return *this;
}

RSAKey::~RSAKey() { if (owner) delete[] owner; }



BigInt RSAKey::modInv(const BigInt& a, const BigInt& m) {
                    
    // euclid extins: gasesc x a.i. a * x congruent 1 (mod m)
    // tin semnul separat pt ca BigInt e unsigned si algoritmul produce coeficienti negativi

    BigInt rest_vechi = a, rest = m;
    BigInt coef_vechi(1), coef(0);
    bool coef_vechi_negativ = false, coef_negativ = false;

    while (!rest.isZero()) {
        BigInt cat = rest_vechi / rest;

        // actualizez restul: rest_nou = rest_vechi - cat * rest
        BigInt temp_rest = rest;
        rest = rest_vechi - cat * rest;
        rest_vechi = temp_rest;

        // actualizez coeficientul: coef_nou = coef_vechi - cat * coef
        // tin semnul separat ca sa nu am underflow pe unsigned
        BigInt cat_coef = cat * coef;
        BigInt temp_coef = coef;
        bool temp_negativ = coef_negativ;

        if (coef_vechi_negativ == coef_negativ) {
            // acelasi semn => scadere
            if (coef_vechi >= cat_coef) {
                coef = coef_vechi - cat_coef;
                coef_negativ = coef_vechi_negativ;
            } else {
                coef = cat_coef - coef_vechi;
                coef_negativ = !coef_vechi_negativ; // se inverseaza semnul
            }
        } else {
            // semne diferite => adunare (minus cu minus da plus)
            coef = coef_vechi + cat_coef;
            coef_negativ = coef_vechi_negativ;
        }

        coef_vechi = temp_coef;
        coef_vechi_negativ = temp_negativ;
    }
     // daca coeficientul e negativ, il aduc in pozitiv adaugand modulul
    if (coef_vechi_negativ)
        coef_vechi = m - coef_vechi;

    return coef_vechi;
}

void RSAKey::generatePrivateKey() {
    n = p*q;
    BigInt phi = (p - BigInt(1)) * (q - BigInt(1));
    d = modInv(e,phi);  
}

ostream& operator<<(ostream& os, const RSAKey& key) {

    os << "Owner: " << key.owner << endl;
    os << "n: " << key.n << endl;
    os << "e: " << key.e << endl;
    os << "d: " << key.d << endl;
    os << "Bit size: " << key.bitSize << endl;
    os << "Key #" << key.keyCount << endl; //asta poate nu e la fel de clar, imi printeaza nr de ordine al cheii
    return os;
}

istream& operator>>(istream& is, RSAKey& key) {

    string ownerName; 
    cout << "Owner: ";
    is >> ownerName;

    delete[] key.owner; //eliberez mem alocata in constructorul cu parametri, ca sa o realoc cu alt owner
    key.owner = new char[ownerName.size() + 1];
    strcpy(key.owner, ownerName.c_str()); //aici il fac const char* ptr ca asa accepta strcpy ca source

    return is;
}

const BigInt& RSAKey::getN() const {
    return this->n;
}

const BigInt& RSAKey::getD() const {    
    return this->d;
}

const BigInt& RSAKey::getE() {
    return e;
}

const char* RSAKey::getOwner() const { 
    return owner;
}

void RSAKey::setOwner(const char* newOwner) {
    delete[] this->owner;
    owner = new char[strlen(newOwner)+1];
    strcpy(owner, newOwner);
}

int RSAKey::getBitSize() const { 
    return bitSize;
}