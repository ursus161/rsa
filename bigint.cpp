#include <iostream>
#include <cstdint>
#include <cstring>
#include <string>
#include <algorithm>
#include <iomanip>


using namespace std;


class BigInt {
private:
        
    static const int MAX_LIMBS = 64; // 4096 biti max

    uint64_t limbs[MAX_LIMBS];
    int size; //  limb-uri sunt folosite efectiv
 
    void trim() {

        while (size > 1 && limbs[size - 1] == 0)
            size--;
    }

public:


    BigInt() {

        memset(limbs, 0, sizeof(limbs)); //fac fiecare byte 0 
        size = 1;
    }

    BigInt(uint64_t val) {

        memset(limbs, 0, sizeof(limbs));
        limbs[0] = val; 

        size = 1;
    }


    BigInt(const string& hex) {

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

   

        bool isZero() const {
            return size == 1 && limbs[0] == 0;
        }


    bool getBit(int i) const {

        int limb_idx = i / 64;
        int bit_idx = i % 64;
        if (limb_idx >= size) return false;
        return (limbs[limb_idx] >> bit_idx) & 1;

    }

    int bitLength() const {

        if (isZero()) return 0; // __builtin_clzll(0) e undefined behavior
 
        return (size - 1) * 64 + (64 - __builtin_clzll(limbs[size - 1]));
    }


    int getSize() const {
        return size;
    }
    //partea de operatori 

    bool operator==(const BigInt& b) const {

    if (size != b.size) return false;

    for (int i = size - 1; i >= 0; i--)
        if (limbs[i] != b.limbs[i]) return false;

    return true;
}

    bool operator!=(const BigInt& b) const {
        return !(*this == b);
    }

    bool operator<(const BigInt& b) const {

        if (size != b.size)
            return size < b.size;

        for (int i = size - 1; i >= 0; i--)
            if (limbs[i] != b.limbs[i])

                return limbs[i] < b.limbs[i];
                
        return false; // egale
    }
    bool operator<=(const BigInt& b) const {

        return *this < b || *this == b;  

    }
    // in rest folosesc supraincarcarea lui < pentru simplitate 
    bool operator>=(const BigInt& b) const {

    return !(*this < b);    

    }

    bool operator>(const BigInt& b) const {
        return b < *this;
        
    }

 

    friend BigInt operator+(const BigInt& a, const BigInt& b) {

        BigInt result;
        uint64_t carry = 0;

        for (int i = 0; i < MAX_LIMBS; i++) {
            __uint128_t sum = (__uint128_t)a.limbs[i] + b.limbs[i] + carry;
            result.limbs[i] = (uint64_t)sum;
            carry = sum >> 64;
        }
        result.size = MAX_LIMBS;
        result.trim();
        return result;
};


    friend BigInt operator-(const BigInt& a, const BigInt& b) {
        
    BigInt result;
    uint64_t borrow = 0;
    for (int i = 0; i < MAX_LIMBS; i++) {
        uint64_t ai = a.limbs[i];
        uint64_t bi = b.limbs[i] + borrow;
        borrow = (ai < bi) ? 1 : 0; // daca ai < bi, atunci trebuie sa imprumutam 1 de la urmatorul limb
        result.limbs[i] = ai - bi;
    }
    result.size = MAX_LIMBS;
    result.trim(); 
    return result;
}

    friend ostream& operator<<(ostream& os, const BigInt& a) { //e prin referinta pentru ca obiectul ostream nu are copy constructor

        os << hex << a.limbs[a.size - 1];

    for (int i = a.size - 2; i >= 0; i--)
        os << setfill('0') << setw(16)<< hex  << a.limbs[i]; //imi afiseaza limburile in hexa

        // care a fost fixul din commitul anterior? *vezi blame* 
        //metoda trim imi elimina tot ce inseamna leading 0 astfel ca metodele functiile setfil si setw imi incarca pe cele 16 spoturi fiecare caracter iar in caz ca am ceva vid pune 0

    os << dec; // daca as vrea sa afisez fara acest 
    return os;
}

    BigInt operator<<( uint64_t shift) const { // shift left  
        int limb_shift = shift / 64;
        int bit_shift = shift % 64;

        for (int i = (*this).getSize() - 1; i >= 0; i--) {
            

        }
    }


friend BigInt operator*(const BigInt& a, const BigInt& b) {
    BigInt result;
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



};



int main() {
  
        cout<< "BigInt(5) bit length: " << BigInt(5).bitLength() << endl;
        BigInt val = 0x20;

        //0x20 =  32 = 100000   
     cout<<(val==BigInt("0x10"))<<endl;
   
        cout<<"hex val: " << BigInt(12) * BigInt(2)<< endl;

        cout<<5<<endl;
       BigInt a("10000000000000005");   // am pe limb1 = 00..1 si pe limb0= 00..005 si imi da trim si face 15, to fix maine
cout << a;   
 
    return 0;   

}