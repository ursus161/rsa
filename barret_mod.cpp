#include <cstdint>

uint64_t modpow_barrett(uint64_t base, uint64_t exp, uint64_t mod)
{
    if (mod == 1) return 0;

    __uint128_t mu = ((__uint128_t)1 << 127) / mod << 1;
    uint64_t mu_hi = (uint64_t)(mu >> 64);
    uint64_t mu_lo = (uint64_t)mu;

    auto reduce = [&](uint64_t a, uint64_t b) {
        __uint128_t z = (__uint128_t)a * b;
        uint64_t z_hi = (uint64_t)(z >> 64);
        uint64_t z_lo = (uint64_t)z;

        __uint128_t mid1 = (__uint128_t)z_hi * mu_lo;
        __uint128_t mid2 = (__uint128_t)z_lo * mu_hi;
        __uint128_t high = (__uint128_t)z_hi * mu_hi;

        __uint128_t mid_sum = mid1 + mid2;
        high += (mid_sum >> 64);

        uint64_t q = (uint64_t)high;
        uint64_t r = z_lo - q * mod;
        if (r >= mod) r -= mod;
        if (r >= mod) r -= mod;
        return r;
    };

    uint64_t result = 1;
    base %= mod;

    while (exp)
    {
        if (exp & 1) {
            result = reduce(result, base);
        }
        base = reduce(base, base);
        exp >>= 1;
    }

    return result;
}