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


    BigInt(const BigInt &obj) { // copy constructor 

        (*this).size = obj.getSize();

        memcpy(limbs, obj.limbs, sizeof(limbs)); // pe fiecare byte din limbs imi pune ce era pe obj.limbs 

        

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
}


     BigInt operator=(const BigInt& obj){ //aveam o greseala aici, o facusem functie friend insa trebuie sa fie metoda pentru ca altfel compilatorul mi ar face un operator= default, ergo coliziune
        
            if (this == &obj) return *this; //sa nu verific degeaba

            memcpy(limbs, obj.limbs, sizeof(limbs));
            size = obj.size;


            return *this;

        
    }

    friend BigInt operator-(const BigInt& a, const BigInt& b) {
    BigInt result;
    uint64_t borrow = 0;
    for (int i = 0; i < MAX_LIMBS; i++) {
        __uint128_t sub = (__uint128_t)a.limbs[i] - b.limbs[i] - borrow; //in caz de oxFFFF.FFF+1 as avea overflow si iau pe 128 biti ca sa trunchiez iar dupa
        result.limbs[i] = (uint64_t)sub;
        borrow = (sub >> 127) & 1;
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

    BigInt operator>>( uint64_t shift) const { // shift right
        int limb_shift = shift / 64;
        int bit_shift = shift % 64;
        BigInt result;

        for (int i = (*this).getSize() - limb_shift- 1;  i >= 0; i--) {
            
                result.limbs[i] = limbs[i+ limb_shift] >> bit_shift;
                result.limbs[i] |= limbs[i + limb_shift + 1] << (64 - bit_shift);
                
        }
        return result;
    }
    
    BigInt operator<<(uint64_t shift) const { //shift left 

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
        BigInt b=a;
        cout<<"a este: " <<a<<endl;
        cout<<"b este: " << b<<endl;


        BigInt x = BigInt("0xFFFFFFFFFFFFFFFF")+BigInt(1); //MERGEEEEEEE (f'ing finally) 

         BigInt numar(16);
         
         numar = x;

        cout<<numar<<endl; // merge fara operator de copiere? 
        cout<<"test la bucata de operator " << x<<endl;

        cout<< "shift "<<(BigInt(16) << 1);
        cout<< "shift la dreapta "<< (BigInt(128) >> 1)<<endl;

        
        //aici am 128 shiftat cu 1 pozitie, deci in hexa imi vine un 16^2. deci 0x100, observam ca e corect


            cout << "16 << 1 = " << (BigInt(16) << 1) << endl;   // x20
    cout << "128 >> 1 = " << (BigInt(128) >> 1) << endl;  // 0x40
    cout << "1 << 64 = " << (BigInt(1) << 64) << endl;    //2^64 (prea lung sa scriu ca hexa)
    cout<< "il shiftez inapoi cu 64 biti (ar trb deci sa mi dea 1): " << ((BigInt(1) << 64) >> 64)<<endl;
    return 0;  

}