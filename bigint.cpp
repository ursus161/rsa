#include <iostream>
#include <cstdint>
#include <cstring>
 

static const int MAX_LIMBS = 64; // 4096 biti max
class BigInt {
public:

    uint64_t limbs[MAX_LIMBS];
    int size; //  limb-uri sunt folosite efectiv

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

    void trim() {

        while (size > 1 && limbs[size - 1] == 0)
            size--;

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

    //partea de operatori 

    bool operator==(const BigInt& b) const {

    if (size != b.size) return false;

    for (int i = size - 1; i >= 0; i--)
        if (limbs[i] != b.limbs[i]) return false;

    return true;
}

    bool operator<(const BigInt& b) const {

        if (size != b.size)
            return size < b.size;

        for (int i = size - 1; i >= 0; i--)
            if (limbs[i] != b.limbs[i])

                return limbs[i] < b.limbs[i];
                
        return false; // egale
    }

    // in rest folosesc supraincarcarea lui < pentru simplitate 
    bool operator>=(const BigInt& b) const {

    return !(*this < b);    

    }

    bool operator>(const BigInt& b) const {
        return b < *this;
    }

};

int main() {
    BigInt a(12345678901234567890ULL);
    BigInt b(9876543210987654321ULL);

    
    return 0;   
}