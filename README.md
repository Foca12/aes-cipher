# AES Crypt - Educational AES Encryption Library

An educational header-only library written in C++ that implements the **AES (Advanced Encryption Standard)** encryption algorithm from scratch.

**Created by:** Foca12
**Creation date:** 2026  
**Language:** C++

## Description

This is an implementation of the AES algorithm, written for educational purposes. This implementation provides AES-128, AES-192 and AES-256 support only in ECB mode.

### Features

- **AES-128**: ECB support for 128-bit encryption
- **AES-192**: ECB support for 192-bit encryption
- **AES-256**: ECB support for 256-bit encryption
- **Cryptographic Operations**:
  - SubBytes (byte substitution)
  - ShiftRows (row rotation)
  - MixColumns (column mixing)
  - AddRoundKey (XOR with the round key)
- **Key Expansion**: Automatic generation of round keys (11 round keys for AES-128, 13 for AES-192, 15 for AES-256)
- **Encryption and Decryption**: Full support for both operations

> Note: This project is educational. Do not use this code in production. See SECURITY.md for details.

## Usage

This library is header-only — include include/aes_cipher.hpp to use the library (no separate compilation required).

### Compilation

```bash
g++ -std=c++17 -o aes_crypt examples/example1.cpp
```

### Examples

See `examples/` for examples.

## 📂 Project Structure

```
aes_cipher/
├── src/                     # Main implementation of AES and core functions
│   ├── encrypt.hpp          # Main encryption functions
│   ├── helpers.hpp          # Helper functions, type definitions and constants
│   ├── message.hpp          # Message splitted into 16 byte-blocks
│   ├── key.hpp              # Key management and key expansion
│   ├── state                # State matrix implementation
│   └── crypt/               # Cryptographic functions
│       ├── add_round_key.hpp
│       ├── sub_bytes.hpp
│       ├── shift_rows.hpp
│       └── mix_columns.hpp
│
├── include/                 # Header-only public API
│   ├── aes_cipher.hpp       # User-needed headers
│   ├── internals.hpp        # Internal management headers
│   └── helpers.hpp          # /src/helpers.hpp header
│
├── examples/
│   ├── example1.cpp         # AES-128 example
│   ├── example2.cpp         # AES-192 example
│   └── example3.cpp         # AES-256 example
│
├── README.md                # This readme
├── SECURITY.md              # Security disclaimer
└── LICENSE                  # MIT License

```

## 🔧 Available Functions

### Multi-Size AES Encryption

```cpp
Message encrypt_aes(Message message, const Key& key)
```

### Multi-Size AES Decryption

```cpp
Message decrypt_aes(Message encrypted, const Key& key)
```

## How AES Works

AES is a symmetric encryption algorithm that uses:

1. **SubBytes**: Nonlinear substitution of each byte
2. **ShiftRows**: Rotation of the rows of the state matrix
3. **MixColumns**: Mixing of columns for diffusion
4. **AddRoundKey**: XOR with the round key
5. **Key Expansion**: Generation of 11 (AES-128), 13 (AES-192) or 15 (AES-256) round keys

For more information: [NIST AES Specification](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197.pdf)

## License

This project is distributed under the **MIT License**—see the [LICENSE](LICENSE) file for details.

Copyright © 2026 Foca12

## Educational Note

This code was written for educational purposes to help understand the details of the AES algorithm. For real-world security applications, use proven cryptographic libraries such as **OpenSSL** or **libsodium**.

---

See also SECURITY.md

**Made with ❤️ from Foca12**
