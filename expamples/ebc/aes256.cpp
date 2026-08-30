#include <iostream>
#include "../../include/aes_cipher.hpp"

using namespace std;

typedef struct {
  string key;
  string plain;
  string cipher;
} test;

const test tests[6] = { 
  // --- Group 1: NIST Vectors ---
  {"603deb1015ca71be2b73aef0857d77811f352c073b6108d72d9810a30914dff4",
   "6bc1bee22e409f96e93d7e117393172a",
   "f3eed1bdb5d2a03c064b5a7e3db181f8"},

  {"603deb1015ca71be2b73aef0857d77811f352c073b6108d72d9810a30914dff4",
   "ae2d8a571e03ac9c9eb76fac45af8e51",
   "591ccb10d410ed26dc5ba74a31362870"},

  {"603deb1015ca71be2b73aef0857d77811f352c073b6108d72d9810a30914dff4",
   "30c81c46a35ce411e5fbc1191a0a52ef",
   "b6ed21b99ca6f4f9f153e7b1beafed1d"},

  {"603deb1015ca71be2b73aef0857d77811f352c073b6108d72d9810a30914dff4",
   "f69f2445df4f9b17ad2b417be66c3710",
   "23304b7a39f9f3ff067d8d8f9e24ecc7"},
  
  // --- Recurring Pattern --- 
  {"000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f",
   "00112233445566778899aabbccddeeff00112233445566778899aabbccddeeff",
   "8ea2b7ca516745bfeafc49904b4960898ea2b7ca516745bfeafc49904b496089"},

  // --- Gruppo 3: All-Zeros Test Vector ---
  {"0000000000000000000000000000000000000000000000000000000000000000",
   "00000000000000000000000000000000",
   "dc95c078a2408989ad48a21492842087"}
};


int main(){
  cout << "START OF THE TEST OF THE LIBRARY (AES-256 EBC)" << endl;
  
  int passed = 0;
  int failed = 0;

  for (int i = 0; i < 6; i++){
    Key key = Key::from_hex(tests[i].key);
    key.expand();

    Message plain = Message::from_hex(tests[i].plain);
    string cipher = tests[i].cipher;

    Message my_cipher = encrypt_aes(plain, key);
    
    Message my_decipher = decrypt_aes(my_cipher, key);

    bool round_error = false;

    if (my_cipher.hex() != cipher){
      cout << "Encrypt error in test number [" << i << "] -> Expected: " << cipher << " | Calculated: " << my_cipher.hex() << endl;
      round_error = true;
    }
    
    if (my_decipher.hex() != plain.hex()){
      cout << "Decrypt error in test number [" << i << "] -> Expected: " << plain.hex() << " | Calculated: " << my_decipher.hex() << endl;
      round_error = true;
    }

    if (!round_error) {
      passed++;
    } else {
      failed++;
    }
  }
  
  cout << "\n=== TEST RESULT ===" << endl;
  cout << "Passed: " << passed << "/6" << endl;
  cout << "Failed: " << failed << "/6" << endl;
  cout << "END OF THE TEST" << endl;
  
  return 0;
}