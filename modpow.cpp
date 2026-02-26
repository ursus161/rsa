#include <iostream>
#include <cstdint>
#include <chrono>

using namespace std;

uint64_t bruteforce(uint64_t base, uint64_t exponent, uint64_t modul) {
   
    uint64_t res = 1; 
    for (uint64_t i = 0; i < exponent; i++) {
        res *= base;
        res %= modul; //pentru a preveni overflow-ul
    }
    return res % modul;
}
uint64_t modpow_cpp(uint64_t base, uint64_t exponent, uint64_t modul) {
    uint64_t result = 1;
    base = base % modul;
    while (exponent > 0) {
        
            if (exponent & 1)
                result = (uint64_t)((__uint128_t)result * base % modul);
            //implementarea branchless se misca cu aprox. 5% mai incet, asa ca lasam compilatorul sa faca optimizarile

        exponent = exponent >> 1;
        base = (uint64_t)((__uint128_t)base * base % modul);
    }
    return result;
}

extern uint64_t modpow_barrett(uint64_t base, uint64_t exp, uint64_t mod);
extern "C" uint64_t modpow(uint64_t base, uint64_t exponent, uint64_t modulus); //asta e functia asm 


bool valid(uint64_t base, uint64_t exponent, uint64_t modul){
    auto cpp_result = modpow_cpp(base, exponent, modul);
    auto result = modpow(base, exponent, modul);
    auto barrett_result = modpow_barrett(base, exponent, modul);

    if(result != cpp_result || result != barrett_result){
        cout << "Error: Results do not match!" << endl;
        cout << "cpp Result: " << cpp_result << endl;
        cout << "asm Result: " << result << endl;
        cout << "barrett Result: " << barrett_result << endl;
        return 0;
    }
    return 1;
}

auto benchmark(uint64_t (*func)(uint64_t, uint64_t, uint64_t), uint64_t base, uint64_t exponent, uint64_t modul) {
    auto start = chrono::high_resolution_clock::now();  
    volatile uint64_t result;
    for (int t = 0; t < 1000000; t++) {
        result = func(base, exponent, modul);
    }
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::microseconds>(end - start).count();
}
#include <cmath>
int main(){

    uint64_t base, exponent, modul;
    base = 987654321;
    exponent = 12345678967;
    modul = (1ULL << 63) - 1; //un numar prim mare, pentru a testa eficienta algoritmului
    // merge cu mod < 2^63

    
    if(!valid(base, exponent, modul))
        return 1;
    
    auto cpp_result = modpow_cpp(base, exponent, modul);
    auto result = modpow(base, exponent, modul);
    auto barrett_result = modpow_barrett(base, exponent, modul);
    //auto bruteforce_result = bruteforce(base, exponent, modul);

  //  uint64_t bruteforce_duration = benchmark(bruteforce, base, exponent, modul);
    uint64_t duration_cpp = benchmark(modpow_cpp, base, exponent, modul);
    uint64_t duration = benchmark(modpow, base, exponent, modul);
    uint64_t duration_barrett = benchmark(modpow_barrett, base, exponent, modul);

    //cout<< "brute force result: " << bruteforce_result << " square&multiply result: " << result << " cpp_result: " << cpp_result << " barrett_result: " << barrett_result << endl;
    cout << "cpp Time for 1M calls: " << duration_cpp << " microseconds" << endl;
    cout << "asm Time for 1M calls: " << duration << " microseconds" << endl;
    cout << "barrett Time for 1M calls: " << duration_barrett << " microseconds" << endl;
   // cout<< "bruteforce Time for 1M calls: " << bruteforce_duration << " microseconds" << endl;

    return 0;

}
 