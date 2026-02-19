#include <cstdint>

uint64_t modpow_barrett(uint64_t base, uint64_t exp, uint64_t mod)
{
    if (mod == 1) return 0;

    uint64_t mu = (~0ULL) / mod; // 0ull e 0 pentru unsgned long long, ~0ULL este 2^64 - 1, deci mu = floor(2^64-1 / mod)

    auto reduce = [&](uint64_t a, uint64_t b) {
        __uint128_t z = (__uint128_t)a * b; // putem avea overflow, de aceea folosim pe 128b
        uint64_t x = (uint64_t)z;
        uint64_t q = (uint64_t)(((__uint128_t)x * mu) >> 64);
        uint64_t r = x - q * mod;

        if (r >= mod) r -= mod;
        if (r >= mod) r -= mod; //matematic 0<=r<2*mod, deci verificam maxim 2 scaderi

        return r;
    };

    uint64_t result = 1;
    base %= mod;

    while (exp)
    {       
        if (exp & 1) {
            result = reduce(result, base);
        }
        
            // result = reduce(result, base)*(exp & 1) + result * !(exp & 1); //branchless are cam 87k microsecunde ptr 1M apeluri
            // iar branched am ~78k microsecunde ptr 1M apeluri 
            // deci compilatorul stie mai bine din nou


        base = reduce(base, base);
        exp >>= 1;
    }

    return result;
}
