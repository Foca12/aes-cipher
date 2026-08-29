# Security Notice

This repository contains an educational implementation of AES (128/192/256) in ECB mode in C++ and is intended only for educational purposes, experimentation, and study.

DO NOT use this code in production or to protect sensitive data.

## What this means

- This library is educational: it is designed to help understand the internal mechanisms of AES and encryption modes.
- Do not expose keys, IVs/nonces, or plaintext in logs.
- Default choices (encryption modes, padding, nonce management) may not be suitable for all security scenarios.

## Why AES ECB isn't secure

This implementation only provides ECB mode. ECB is insecure for multi-block data: identical plaintext blocks encrypt to identical ciphertext blocks, which leaks plaintext structure and enables tampering. Do not use ECB in production — use an authenticated mode such as AES-GCM or ChaCha20-Poly1305 instead.

## Raccomandations

If you are writing production code use a secure library such as OpenSSL, libsodium, BoringSSL or Botan

## Limitation of liability

THE MAINTAINER ASSUMES NO LIABILITY FOR DAMAGES ARISING FROM THE USE OF THIS SOFTWARE. USE IS AT THE USER'S OWN RISK.

---

Foca12 — 2026
