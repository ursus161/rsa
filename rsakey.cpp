#include <iostream>
#include "bigint.h"

class RSAKey {

//deci in primul rand vreau sa explic logica, pentru ca poate fi confuzing, 
//trb sa iau 2 numere prime, de obicei se aleg mari pt a fi mai usor de ghicit, p si q si fac n = p * q
//phi (nu o sa caut simbolul grecesc :) ) = (q-1)*(p-1) e functia lui euler
//apoi se ia un exponent e, conventia e sa fie 65537, care la prima vedere pare in jur de 2^16, si chiar este 2^16+1, la o cautare amanuntita aflam ca e al cincilea nr al lui fermat
// mai am si d care e inversul modular al lui e mod phi, deci fac un e*d%phi = 1

private:

    static const BigInt e  ; //o sa ramana mereu acelasi, e standard asa ca nu l mai recreez de fiecare data
    BigInt p,q;
    BigInt n;  
    BigInt d;
    const int bitSize;
    char *owner;
    static int keyCount; //nr de chei totale

public:

   RSAKey(const BigInt& p, const BigInt& q, const char* ownerName)  : bitSize(p.bitLength() + q.bitLength()) {

        this->p = p;
        this->q = q;
        n = p * q;
        
        owner = new char[strlen(ownerName) + 1];
        strcpy(owner, ownerName);
        
        keyCount++;
    }


      RSAKey(const RSAKey& obj) :   bitSize(obj.bitSize) {// copy constructorul

        this->p = obj.p;
        this->q = obj.q;
        this->n = obj.n;
        this->d = obj.d;

        owner = new char[strlen(obj.owner) + 1];
        strcpy(owner, obj.owner);

        keyCount++;
    }

        RSAKey& operator=(const RSAKey& obj) {

            if (this == &obj) return *this;

            this->p = obj.p;
            this->q = obj.q;

            this->n = obj.n;
            this->d = obj.d;

            delete[] owner; // obiectul daca nu l eliberez acm mi s ar elibera memoria de abia la final de lifetime 

            owner = new char[strlen(obj.owner) + 1];
            strcpy(owner, obj.owner);

            return *this;
}



        static BigInt modInv(const BigInt& a, const BigInt& m) {
                        
            BigInt result(1); //brute-force momentan deci iau pe numere mici 
            while (!((a * result) % m == BigInt(1))) {
                result = result + BigInt(1);
            }
            return result;
}


        void generate()
        {
            n = p*q;
            BigInt phi = (p - BigInt(1)) * (q - BigInt(1));
            d = modInv(e,phi);  
        }



    


        friend ostream& operator<<(ostream& os, const RSAKey& key) {

            os << "Owner: " << key.owner << endl;
            os << "n: " << key.n << endl;
            os << "e: " << key.e << endl;
            os << "d: " << key.d << endl;
            os << "Bit size: " << key.bitSize << endl;
            os << "Key #" << key.keyCount << endl; //asta poate nu e la fel de clar, imi printeaza nr de ordine al cheii
            return os;
}



        friend istream& operator>>(istream& is, RSAKey& key){

             string ownerName; 
            cout << "Owner: ";
            is >> ownerName;

            delete[] key.owner; //eliberez mem alocata in constructorul cu parametri, ca sa o realoc cu alt owner
            key.owner = new char[ownerName.size() + 1];
            strcpy(key.owner, ownerName.c_str()); //aici il fac const char* ptr ca asa accepta strcpy ca source

            return is;
        }


        const BigInt& getN() const{
            return this->n;
        }

        const BigInt& getD() const{
            return this->d;
        }


        static const BigInt& getE(){
            return e;
        }

        const char* getOwner() const { 
            return owner; }
            
        void setOwner(const char* newOwner) {


            delete[] this->owner;
            owner = new char[strlen(newOwner)+1];
            strcpy(owner, newOwner);
        }
        int getBitSize() const 
        { 
            return bitSize; // aici 
         }


    ~RSAKey(){ if (owner) delete[] owner;}



};  

const BigInt RSAKey::e = BigInt(65537);

int RSAKey::keyCount = 0;

int main(){

    cout<<RSAKey::modInv(BigInt(2),BigInt(3))<<endl;
    return 0;
}