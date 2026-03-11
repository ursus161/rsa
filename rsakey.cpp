class RSAKey {

//deci in primul rand vreau sa explic logica, pentru ca poate fi confuzing, 
//trb sa iau 2 numere prime, de obicei se aleg mari pt a fi mai usor de ghicit, p si q si fac n = p * q
//phi (nu o sa caut simbolul grecesc :) ) = (q-1)*(p-1) e functia lui euler
//apoi se ia un exponent e, conventia e sa fie 65537, care la prima vedere pare in jur de 2^16, si chiar este 2^16+1, la o cautare amanuntita aflam ca e al cincilea nr al lui fermat
// mai am si d care e inversul modular al lui e mod phi, deci fac un e*d%phi = 1

private:

    const static BigInt e   ;
    BigInt p,q;
    BigInt n;  
    BigInt d;
   static const int bitSize = 4096;
    char *owner;
    static int keyCount;

public:

    RSAKey(){
        owner = nullptr;
        
    }

    ~RSAKey(){ if (owner) delete[] owner;}

};