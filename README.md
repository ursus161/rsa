# RSA Cryptosystem with Custom BigInt Library

A C++ implementation of the RSA cryptographic algorithm, developed to demonstrate low-level modular arithmetic, number theory, and object-oriented design. The project includes a custom BigInt library to handle the large-scale integer operations required for cryptographic security without relying on external libraries like OpenSSL or GMP.

## Technical Features

### 1. Arbitrary-Precision Arithmetic (BigInt)
The `BigInt` class manages large integers as arrays of 64-bit unsigned limbs. Key implementation details include:
* **Limb-based Storage**: Uses a fixed-size array of 128 limbs (up to 8192 bits) to balance performance and memory safety.
* **Karatsuba Multiplication**: Implements a recursive $O(n^{1.585})$ algorithm for multiplication, which is automatically triggered when the input size exceeds a specific threshold.
* **Barrett Reduction**: Optimizes modular exponentiation (`modpow`) by replacing expensive division operations with a sequence of multiplications and bit shifts.
* **Binary Exponentiation**: Uses the square-and-multiply method for efficient $m^e \pmod n$ calculations.

### 2. RSA Engine and Key Generation
* **Key Generation**: Computes the public modulus $n = p \times q$ and generates the private exponent $d$ using a custom Extended Euclidean Algorithm for modular inversion.
* **Cryptographic Primitives**: Implements standard RSA encryption and decryption where $c = m^e \pmod n$ and $m = c^d \pmod n$.
* **Benchmarking System**: Integrated performance monitoring that calculates the geometric mean of execution times across multiple iterations. It utilizes the `volatile` keyword to ensure the compiler does not optimize away mathematical operations during tests.

### 3. Message Handling
The `Message` class provides a wrapper for plaintext strings, managing the conversion between ASCII characters and `BigInt` blocks. This allows full text strings to be processed as a sequence of independent cryptographic blocks.

## Build and Usage

### Compilation
The project includes a `Makefile` configured for optimized builds using `g++`.

```bash
# Compile the project
make

# Run the application
./rsa