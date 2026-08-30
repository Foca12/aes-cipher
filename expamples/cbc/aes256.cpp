#include <iostream>
#include "../../include/aes_cipher.hpp"

using namespace std;

typedef struct {
  string key;
  string iv;
  string plain;
  string cipher;
} test;

const test tests[6] = {
  // --- NIST Vectors ---
    {"603deb1015ca71be2b73aef0857d77811f352c073b6108d72d9810a30914dff4",
    "000102030405060708090a0b0c0d0e0f",
    "6bc1bee22e409f96e93d7e117393172a",
    "f58c4c04d6e5f1ba779eabfb5f7bfbd6"},

    {"603deb1015ca71be2b73aef0857d77811f352c073b6108d72d9810a30914dff4",
    "f58c4c04d6e5f1ba779eabfb5f7bfbd6",
    "ae2d8a571e03ac9c9eb76fac45af8e51",
    "9cfc4e967edb808d679f777bc6702c7d"},

    {"603deb1015ca71be2b73aef0857d77811f352c073b6108d72d9810a30914dff4",
    "9cfc4e967edb808d679f777bc6702c7d",
    "30c81c46a35ce411e5fbc1191a0a52ef",
    "39f23369a9d9bacfa530e26304231461"},

    {"603deb1015ca71be2b73aef0857d77811f352c073b6108d72d9810a30914dff4",
    "39f23369a9d9bacfa530e26304231461",
    "f69f2445df4f9b17ad2b417be66c3710",
    "b2eb05e2c39be9fcda6c19078c6a9d1b"},
    
  // --- Recurring Pattern --- 
  {"000102030405060708090a0b0c0d0e0f000102030405060708090a0b0c0d0e0f",
   "73bed6b8e3c1743b7116e69e22229516",
   "00112233445566778899aabbccddeeff00112233445566778899aabbccddeeff",
   "a7fb871942f83dc8115554cd4a576faea5304b298679d58e28af6fb781385947"},

  // --- All-Zeros Test Vector ---
  {"0000000000000000000000000000000000000000000000000000000000000000",
   "00000000000000000000000000000000",
   "00000000000000000000000000000000",
   "dc95c078a2408989ad48a21492842087"}
};


int main(){
  cout << "START OF THE TEST OF THE LIBRARY (AES-256 CBC)" << endl;
  
  int passed = 0;
  int failed = 0;

  for (int i = 0; i < 6; i++){
    Key key = Key::from_hex(tests[i].key);
    key.expand();
    
    Message plain = Message::from_hex(tests[i].plain);
    State iv = State::from_hex(tests[i].iv);
    string cipher = tests[i].cipher;

    Message my_cipher = encrypt_aes(plain, key, iv);
    
    Message my_decipher = decrypt_aes(my_cipher, key, iv);

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