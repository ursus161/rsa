 
#include <cstdint>
 
using namespace std;

typedef unsigned __int128 uint128_t;

uint64_t barrett_mod(uint128_t a, uint64_t mod, uint128_t recip) {
   
    uint64_t q = (uint64_t)((a * recip) >> 128);
    uint64_t r = (uint64_t)a - q * mod;
    return r >= mod ? r - mod : r;
}

uint64_t modpow_barrett(uint64_t base, uint64_t exp, uint64_t mod) {
    uint128_t recip = ((uint128_t)1 << 127) / mod << 1;

    uint64_t result = 1;
    base = barrett_mod(base, mod, recip);

    while (exp > 0) {
        if (exp & 1)
            result = barrett_mod((uint128_t)result * base, mod, recip);
        base = barrett_mod((uint128_t)base * base, mod, recip);
        exp >>= 1;
    }
    return result;
}

 