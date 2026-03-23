#include "bigint.h"

bool BigInt::hexmode = false;

void BigInt::trim() {
    while (size > 1 && limbs[size - 1] == 0)
        size--;
}

BigInt::BigInt() {
    memset(limbs, 0, sizeof(limbs)); //fac fiecare byte 0 
    size = 1;
}

BigInt::BigInt(uint64_t val) {
    memset(limbs, 0, sizeof(limbs));
    limbs[0] = val; 
    size = 1;
}

BigInt::BigInt(const string& hex) {
    memset(limbs, 0, sizeof(limbs));
    size = 0;
    int i = hex.size();

    while (i > 0) {
        int start = max(0, i - 16);
        string chunk = hex.substr(start, i - start);
        limbs[size++] = stoull(chunk, nullptr, 16);
        i = start;
    }
    if (size == 0) size = 1;
}

BigInt::BigInt(const BigInt &obj) { // copy constructor 
    
    (*this).size = obj.getSize();
    memcpy(limbs, obj.limbs, sizeof(limbs)); // pe fiecare byte din limbs imi pune ce era pe obj.limbs 
}

BigInt::~BigInt() {}

void BigInt::setHex() { hexmode = true; }
void BigInt::setDec() { hexmode = false; }

bool BigInt::isZero() const {
    return size == 1 && limbs[0] == 0;
}

bool BigInt::getBit(int i) const {
    int limb_idx = i / 64;
    int bit_idx = i % 64;
    if (limb_idx >= size) return false;
    return (limbs[limb_idx] >> bit_idx) & 1;
}

void BigInt::setBit(int i) {
    limbs[i / 64] |= (1ULL << (i % 64));
}

int BigInt::bitLength() const {
    if (isZero()) return 0; // __builtin_clzll(0) e undefined behavior
    return (size - 1) * 64 + (64 - __builtin_clzll(limbs[size - 1]));
}

int BigInt::getSize() const {
    return size;
}


uint64_t getLimb(int i) const{
    return limbs[i];
}

bool BigInt::operator==(const BigInt& b) const {
    if (size != b.size) return false;
    for (int i = size - 1; i >= 0; i--)
        if (limbs[i] != b.limbs[i]) return false;
    return true;
}

bool BigInt::operator!=(const BigInt& b) const {
    return !(*this == b);
}

bool BigInt::operator<(const BigInt& b) const {
    if (size != b.size)
        return size < b.size;
    for (int i = size - 1; i >= 0; i--)
        if (limbs[i] != b.limbs[i])
            return limbs[i] < b.limbs[i];
    return false; // egale
}

bool BigInt::operator<=(const BigInt& b) const {
    return *this < b || *this == b;  
}

bool BigInt::operator>=(const BigInt& b) const {
    return !(*this < b);    
}

bool BigInt::operator>(const BigInt& b) const {
    return b < *this;
}

BigInt operator+(const BigInt& a, const BigInt& b) {
    BigInt result;
    uint64_t carry = 0;
    for (int i = 0; i < BigInt::MAX_LIMBS; i++) {
        __uint128_t sum = (__uint128_t)a.limbs[i] + b.limbs[i] + carry;
        result.limbs[i] = (uint64_t)sum;
        carry = sum >> 64;
    }
    result.size = BigInt::MAX_LIMBS;
    result.trim();
    return result;
}

BigInt& BigInt::operator=(const BigInt& obj) {
    if (this == &obj) return *this; //sa nu verific degeaba
    memcpy(limbs, obj.limbs, sizeof(limbs));
    size = obj.size;
    return *this;
}

BigInt operator-(const BigInt& a, const BigInt& b) {
    BigInt result;
    uint64_t borrow = 0;
    for (int i = 0; i < BigInt::MAX_LIMBS; i++) {
        __uint128_t sub = (__uint128_t)a.limbs[i] - b.limbs[i] - borrow; //in caz de oxFFFF.FFF+1 as avea overflow si iau pe 128 biti ca sa trunchiez iar dupa
        result.limbs[i] = (uint64_t)sub;
        borrow = (sub >> 127) & 1;
    }
    result.size = BigInt::MAX_LIMBS;
    result.trim();
    return result;
}

ostream& operator<<(ostream& os, const BigInt& a) {
    os << hex << a.limbs[a.size - 1];
    for (int i = a.size - 2; i >= 0; i--)
        os << setfill('0') << setw(16) << hex << a.limbs[i]; //imi afiseaza limburile in hexa
        // care a fost fixul din commitul anterior? *vezi blame* 
        //metoda trim imi elimina tot ce inseamna leading 0 astfel ca metodele functiile setfil si setw imi incarca pe cele 16 spoturi fiecare caracter iar in caz ca am ceva vid pune 0
    os << dec; // daca as vrea sa afisez fara acest 
    return os;
}

BigInt BigInt::operator>>(uint64_t shift) const { // shift right
    int limb_shift = shift / 64;
    int bit_shift = shift % 64;
    BigInt result;

    for (int i = (*this).getSize() - limb_shift- 1;  i >= 0; i--) {
        result.limbs[i] = limbs[i+ limb_shift] >> bit_shift;
        if(limb_shift+i+1 < (*this).getSize() && bit_shift >  0 ) //ptr prima conditie citesc garbage altfel, prob segfault chiar in func de compiler 
        {
            result.limbs[i] |= limbs[i + limb_shift + 1] << (64 - bit_shift);
        }
    }
    result.size = max(1, size-limb_shift); //evident nu pot sa am size<1, asta trebuie sa iau pentru cazuri precum 5>>128 unde as ajunge la size = -2, dar result = 0, deci size=1
    return result;
}

BigInt BigInt::operator<<(uint64_t shift) const { //shift left 
    int limb_shift = shift / 64;
    int bit_shift = shift % 64;
    BigInt result;
    uint64_t carry = 0; 
    for (int i = 0; i <=(*this).getSize() - 1; i++) {
        result.limbs[i + limb_shift] = (limbs[i] << bit_shift) | carry; // mut i pe i+limbul de shift apoi adaug carry ul prin | carry
        carry =( bit_shift > 0)*(limbs[i] >> (64 - bit_shift)); //carry ul e ce da overflow pe un limb si se muta pe urmatorul, dcriu ca mux 
        //dau update la carry dupa, ca sa pun mai intai carry ul initial
    }

    result.limbs[size + limb_shift] = carry*(carry!=0); //asta e ult carry, tot mux pt constant time
    result.size = size + limb_shift + (carry!=0);
    result.trim();
    return result;
}

BigInt operator*(const BigInt& a, const BigInt& b) {
    BigInt result;

    if (a.getSize() + b.getSize() > BigInt::MAX_LIMBS)
        throw runtime_error("BigInt overflow in operator* " );

    for (int i = 0; i < a.getSize(); i++) {
        uint64_t carry = 0;
        for (int j = 0; j < b.getSize(); j++) {
            __uint128_t prod = (__uint128_t)a.limbs[i] * b.limbs[j] + result.limbs[i + j] + carry;
            result.limbs[i + j] = (uint64_t)prod;
            carry = (uint64_t)(prod >> 64);
        }
        result.limbs[i + b.getSize()] += carry;
    }
    result.size = a.getSize() + b.getSize();
    result.trim();
    return result;
}

pair<BigInt, BigInt> BigInt::divmod(const BigInt& divisor) const {
    // SURSA PENTRU IMPLEMENTAREA LOGICII ALGORITMULUI: https://en.wikipedia.org/wiki/Division_algorithm

    if (divisor.isZero()) throw runtime_error("Division by zero"); //evident nu pot sa impart la 0
    if (*this < divisor) return {BigInt(0), *this}; // daca am 1/7 de exemplu , o sa am cat 0 si rest 7

    BigInt catul;
    BigInt rest;

    for (int i = bitLength() - 1; i >= 0; i--) { // aici o iau de la msb, cumva ca pe hartie incep shiftarea la stanga si iau bit cu bit 

        rest = rest << 1; // fac iar lsb zero si l adaug in if ul urm, cobor ca pe "foaie" cumva urmatorul bit 

        if (getBit(i))
            rest.limbs[0] |= 1; // aici adaug ....1 daca pe bit ul din pasul asta e se

        if (rest >= divisor) { // matematic not good

            uint64_t limb_id = i/64;
            uint64_t bit_id =  i%64;

            rest = rest - divisor;
            catul.setBit(i);
        }
    }

    catul.size = (bitLength() / 64) + 1; // bitlen // 64 sunt limburile pline si +1 e restul din limbul final
    catul.trim(); // aici as putea avea un limb de genul [1111..1] si sa nu mai am nimic pe urmatorul, iar prin linia 301 as avea size 2 cand trebuie 1, insa corectez asta din trim()
    rest.trim();
    return {catul, rest};
}

BigInt BigInt::operator/(const BigInt& obj) const {
    return divmod(obj).first; 
}

BigInt BigInt::operator%(const BigInt& obj) const {
    return divmod(obj).second;
}