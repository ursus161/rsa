#pragma once
#include <iostream>
#include <cstdint>
#include <cstring>
#include <string>
#include <algorithm>
#include <iomanip>

using namespace std;


class BigInt {
private:
        
    static const int MAX_LIMBS = 128; // 4096 biti max, insa am dimensiune dubla ptr ca hamming_weight(n*m) = n+m
    static bool hexmode;

    uint64_t limbs[MAX_LIMBS];
    int size; //  limb-uri sunt folosite efectiv
 
    void trim();

public:

    BigInt();
    BigInt(uint64_t val);
    BigInt(const string& hex);
    BigInt(const BigInt &obj); // copy constructor 
    ~BigInt(); //n am alocat nimic pe heap, nici nu sterg insa scriu destructorul BigInt din motive de cerinte de proiect 

    static void setHex();
    static void setDec(); //le fac statice ptr ca vreau toate afisarile sa fie ori hex ori in dec, nu vreau sa am unele intr un fel iar altele in alt fel

    bool isZero() const;
    bool getBit(int i) const;
    void setBit(int i); //antetul e explicit, setez bitul de pe pozitia i de la lsb incepand numaratoarea
    int bitLength() const;
    int getSize() const;

    //partea de operatori 
    bool operator==(const BigInt& b) const;
    bool operator!=(const BigInt& b) const;
    bool operator<(const BigInt& b) const;
    bool operator<=(const BigInt& b) const;
    // in rest folosesc supraincarcarea lui < pentru simplitate 
    bool operator>=(const BigInt& b) const;
    bool operator>(const BigInt& b) const;

    friend BigInt operator+(const BigInt& a, const BigInt& b);
    BigInt& operator=(const BigInt& obj); //aveam o greseala aici, o facusem functie friend insa trebuie sa fie metoda pentru ca altfel compilatorul mi ar face un operator= default, ergo coliziune
    friend BigInt operator-(const BigInt& a, const BigInt& b);
    friend ostream& operator<<(ostream& os, const BigInt& a); //e prin referinta pentru ca obiectul ostream nu are copy constructor

    // !! ceva ce as vrea sa subliniez la aceste shifturi este ca pe tipuri predefinite precum uint64_t, am UD pe lucruri precum 5>>64, insa eu am 0 aici, vreau sa minimizez UD urile
    BigInt operator>>(uint64_t shift) const; // shift right
    BigInt operator<<(uint64_t shift) const; //shift left 

    friend BigInt operator*(const BigInt& a, const BigInt& b);

    pair<BigInt, BigInt> divmod(const BigInt& divisor) const; // SURSA PENTRU IMPLEMENTAREA LOGICII ALGORITMULUI: https://en.wikipedia.org/wiki/Division_algorithm
    BigInt operator/(const BigInt& obj) const;
    BigInt operator%(const BigInt& obj) const;

    //varianta de barret precedenta imi rula pe 64 de biti, nu scala pentru obiecte de tip BigInt
    //sursa logicii de implementare a algoritmului : https://www.nayuki.io/page/barrett-reduction-algorithm
    // modpow ramane inline din cauza lambda-ului intern
    BigInt modpow(const BigInt& exp, const BigInt& mod) const {

        if (mod == BigInt(1)) return BigInt(0);

        int k = mod.bitLength();
        BigInt mu = (BigInt(1) << (2 * k)) / mod;

        auto reduce = [&](const BigInt& a, const BigInt& b) -> BigInt {
            BigInt z = a * b;
            BigInt q = (z * mu) >> (2 * k);
            BigInt r = z - q * mod;
            while (r >= mod) r = r - mod;
            return r;   
        };

        BigInt result(1);
        BigInt base = *this % mod;

        for (int i = exp.bitLength() - 1; i >= 0; i--) {
            result = reduce(result, result);
            if (exp.getBit(i))
                result = reduce(result, base);
        }
        return result;
    }
};