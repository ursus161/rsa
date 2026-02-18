#include <iostream>
#include <cstdint>
#include <chrono>

using namespace std;

int modpow_cpp(uint64_t base, uint64_t exponent, uint64_t modul) {
    uint64_t result = 1;
    base = base % modul;
    while (exponent > 0) {
       
        result = ((result * base) % modul)*(exponent & 1) + result * ((exponent & 1) ^ 1);
        //branchless  

        exponent = exponent >> 1;
        base = (base * base) % modul;
    }
    return result;
}

extern "C" uint64_t modpow(uint64_t base, uint64_t exponent, uint64_t modulus);


bool valid(uint64_t base, uint64_t exponent, uint64_t modul){
    auto cpp_result = modpow_cpp(base, exponent, modul);
    auto result = modpow(base, exponent, modul);

    if(result != cpp_result){
        cout << "Error: Results do not match!" << endl;
        cout << "cpp Result: " << cpp_result << endl;
        cout << "asm Result: " << result << endl;
        return 0;
    }
    return 1;
}

int main(){
    uint64_t base, exponent, modul;
    cout << "Enter base: ";
    cin >> base;
    cout << "Enter exponent: ";
    cin >> exponent;
    cout << "Enter modulus: ";
    cin >> modul;
    
    auto start = chrono::high_resolution_clock::now();
    volatile uint64_t result;
    //volatile is used to prevent the compiler from optimizing

    for(int t = 0; t < 1000000; t++){
        result = modpow(base, exponent, modul);
    }
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);


    auto start_cpp = chrono::high_resolution_clock::now();
    volatile uint64_t cpp_result;
    for(int t = 0; t < 1000000; t++){
        cpp_result = modpow_cpp(base, exponent, modul);
    }
    auto end_cpp = chrono::high_resolution_clock::now();
    auto duration_cpp = chrono::duration_cast<chrono::microseconds>(end_cpp - start_cpp);

 
    if(!valid(base, exponent, modul)){
        return 1;
    }

    cout << "Result: " << result << endl;
    cout << "cpp Time for 1M calls: " << duration_cpp.count() << " microseconds" << endl;
    cout << "asm Time for 1M calls: " << duration.count() << " microseconds" << endl;
    return 0;

}
// nasm -f elf64 modpow.asm -o modpow.o
// g++ -o test modpow.cpp modpow.o -no-pie
// ./test
 