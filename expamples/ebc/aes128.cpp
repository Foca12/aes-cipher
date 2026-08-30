#include <iostream>
#include "../../include/aes_cipher.hpp"

using namespace std;

typedef struct {
  string key;
  string plain;
  string cipher;
} test;

const test tests[6] = {
  // --- NIST Vectors ---
  {
    "2b7e151628aed2a6abf7158809cf4f3c",
    "6bc1bee22e409f96e93d7e117393172a",
    "3ad77bb40d7a3660a89ecaf32466ef97"
    },
  {
    "2b7e151628aed2a6abf7158809cf4f3c",
    "ae2d8a571e03ac9c9eb76fac45af8e51",
    "f5d3d58503b9699de785895a96fdbaaf",
    },
  {
    "2b7e151628aed2a6abf7158809cf4f3c",
    "30c81c46a35ce411e5fbc1191a0a52ef",
    "43b1cd7f598ece23881b00e3ed030688",

    },
  {
    "2b7e151628aed2a6abf7158809cf4f3c",
    "f69f2445df4f9b17ad2b417be66c3710",
    "7b0c785e27e8ad3f8223207104725dd4",
    },
  
  // --- Recurring Pattern --- 
  {
    "000102030405060708090a0b0c0d0e0f",
    "00112233445566778899aabbccddeeff00112233445566778899aabbccddeeff",
    "69c4e0d86a7b0430d8cdb78070b4c55a69c4e0d86a7b0430d8cdb78070b4c55a"
    },

  // --- All-Zeros Test Vector ---
  {
    "00000000000000000000000000000000",
    "00000000000000000000000000000000",
    "66e94bd4ef8a2c3b884cfa59ca342b2e"
    }
};


int main(){
  cout << "START OF THE TEST OF THE LIBRARY (AES-128 EBC)" << endl;
  
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