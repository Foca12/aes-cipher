#include <iostream>
#include "../include/aes_cipher.hpp"

using namespace std;

typedef struct {
  string key;
  string plain;
  string cipher;
} test;

const test tests[6] = {
  {
    // --- NIST Vectors ---
    "10a58869d74be5a374cf867cfb473859",
    "00000000000000000000000000000000",
    "6d251e6944b051e04eaa6fb4dbf78465"
    },
  {
    "caea65cdbb751f983ce0f652aee7800c",
    "ffffffffffffffffffffffffffffffff",
    "110d17361ec68a18f17be8b0ec7887b1"
    },
  {
    "2b7e151628aed2a6abf7158809cf4f3c",
    "f3c2170a4de154785409899abe67c355203c53e3542232ab4595844090c20a40",
    "a2797e55a0617e6cb0e1a5ff869865042a0015cdc4bf65133188aad2829455ec"
    },
  {
    "fedcba9876543210fedcba9876543210",
    "0123456789abcdeffedcba98765432100123456789abcdeffedcba9876543210",
    "9b680aecaa39f6a1b12439b1cd03421a9b680aecaa39f6a1b12439b1cd03421a"
    },
  {
    "78563412bc9a78563412bc9a78563412",
    "ab1b28010abe8d0fead0e3a981f152a0",
    "8b15f277caa9f9892a6ea03d75322393"
    },

  // --- All-Zeros Test Vector ---
  {
    "00000000000000000000000000000000",
    "00000000000000000000000000000000",
    "66e94bd4ef8a2c3b884cfa59ca342b2e"
    }
};


int main(){
  cout << "START OF THE TEST OF THE LIBRARY (AES-128)" << endl;
  
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