#include <iostream>
#include <cstdint>

using namespace std;


extern "C" uint64_t modpow(uint64_t base, uint64_t exponent, uint64_t modulus);

int main(){
    uint64_t base, exponent, modul;
    cout << "Enter base: ";
    cin >> base;
    cout << "Enter exponent: ";
    cin >> exponent;
    cout << "Enter modulus: ";
    cin >> modul;
    
    uint64_t result = modpow(base, exponent, modul);

    cout << "Result: " << result << endl;
    return 0;

}
// nasm -f elf64 modpow.asm -o modpow.o
// g++ -o test modpow.cpp modpow.o -no-pie
// ./test
 