# modpow

Modular exponentiation (`base^exp % mod`) in x86-64 assembly with branchless `cmov`, benchmarked against C++ and Barrett reduction which eliminates the expensive `div` instruction.

## Benchmark (1M calls, `-O3`)

| Implementation | Time (μs) |
|---|---|
| x86-64 ASM (div) | 94,264 |
| C++ (div) | 75,825 |
| **C++ Barrett (no div)** | **16,299** |

Barrett alg replaces `div` (20-90 cycles) with `mul` + `shift` + `sub`, achieving ~5.8x speedup over naive assembly.

## Build

```bash
make clean && make run
```