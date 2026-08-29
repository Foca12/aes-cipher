#include <iostream>
#include "../include/aes_cipher.hpp"

using namespace std;

typedef struct {
  string key;
  string plain;
  string cipher;
} test;

const test tests[6] = { 
  // --- NIST Vectors ---
  {"8e73b0f7da0e6452c810f32b809079e562f8ead2522c6b7b",
   "6bc1bee22e409f96e93d7e117393172a",
   "bd334f1d6e45f25ff712a214571fa5cc"},

  {"8e73b0f7da0e6452c810f32b809079e562f8ead2522c6b7b",
   "ae2d8a571e03ac9c9eb76fac45af8e51",
   "974104846d0ad3ad7734ecb3ecee4eef"},

  {"8e73b0f7da0e6452c810f32b809079e562f8ead2522c6b7b",
   "30c81c46a35ce411e5fbc1191a0a52ef",
   "ef7afd2270e2e60adce0ba2face6444e"},

  {"8e73b0f7da0e6452c810f32b809079e562f8ead2522c6b7b",
   "f69f2445df4f9b17ad2b417be66c3710",
   "9a4b41ba738d6c72fb16691603c18e0e"},

   
  {"000102030405060708090a0b0c0d0e0f1011121314151617",
   "00112233445566778899aabbccddeeff",
   "dda97ca4864cdfe06eaf70a0ec0d7191",},

  // --- All-Zeroes Test Vector
  {"000000000000000000000000000000000000000000000000",
   "00000000000000000000000000000000",
   "aae06992acbf52a3e8f4a96ec9300bd7",}
};


int main(){
  cout << "START OF THE TEST OF THE LIBRARY (AES-192)" << endl;
  
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