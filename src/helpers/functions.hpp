#pragma once

#include "constants.hpp"
#include "types.hpp"

namespace aes_functions
{
  aes_types::state_row shift_row_left(const aes_types::state_row& row, uint8_t rounds){
    rounds %= aes_constants::state_columns;
    if (rounds == 0){
      return row;
    }

    aes_types::state_row result;

    // left character becomes right character
    for (size_t i = 0; i < aes_constants::state_columns; i ++){
      result[i] = row[(i+rounds) % aes_constants::state_columns];
    }
    return result;
  }
  aes_types::state_row shift_row_right(const aes_types::state_row& row, uint8_t rounds){
    rounds %= aes_constants::state_columns;
    if (rounds == 0){
      return row;
    }

    aes_types::state_row result;

    // right character becomes left character
    for (int i = aes_constants::state_columns-1; i >= 0; i --){
      result[i] = row[i-rounds >= 0? i-rounds : aes_constants::state_columns+i-rounds];
    }
    return result;
  }
  
  int xtime(int x){
    bool is_one = x & 0x80;
    x = x << 1 & 0xFF;
    return is_one? x ^ 0x1b : x ;
  }

  int mul_01(int x) {return x;}
  int mul_02(int x) {return xtime(x);}
  int mul_03(int x) {return mul_02(x) ^ (x);}
  int mul_04(int x) {return mul_02(mul_02(x));}
  int mul_08(int x) {return mul_02(mul_04(x));}
  int mul_09(int x) {return mul_08(x) ^ x;}
  int mul_0B(int x) {return mul_08(x) ^ mul_02(x) ^ x;}
  int mul_0D(int x) {return mul_08(x) ^ mul_04(x) ^ x;}
  int mul_0E(int x) {return mul_08(x) ^ mul_04(x) ^ mul_02(x);}

  const std::function<int(int)> mul_matrix[aes_constants::state_rows][aes_constants::state_columns] = \
          {{mul_02, mul_03, mul_01, mul_01},\
           {mul_01, mul_02, mul_03, mul_01},\
           {mul_01, mul_01, mul_02, mul_03},\
           {mul_03, mul_01, mul_01, mul_02}};
  const std::function<int(int)> inv_mul_matrix[aes_constants::state_rows][aes_constants::state_columns] = \
            {{mul_0E, mul_0B, mul_0D, mul_09},\
             {mul_09, mul_0E, mul_0B, mul_0D},\
             {mul_0D, mul_09, mul_0E, mul_0B},\
             {mul_0B, mul_0D, mul_09, mul_0E}};


  std::string convert_to_string(aes_types::ilist vct){  
    std::stringstream ss;
    std::string str;
    bool only_valid = true;
    
    for (int i : vct) {
      if ((i < 32 || i > 126) && i != 0){
        only_valid = false;
        }
      }

      for (int i : vct) {
        if (only_valid){
          str += (char) i;
        }
        else {
          ss << "\\x";
          ss << std::setfill('0') << std::setw(2) << std::hex << i;
        }
      }

      return only_valid? str : ss.str();
    }
  template <size_t len>
  std::string convert_to_string(aes_types::iarr<len> arr){  
    aes_types::ilist vct;
    vct.assign(arr.begin(), arr.end());
    return convert_to_string(vct);
  }

  // convert hex/oct string in vector of uint
  aes_types::ilist basic_from_hex(const std::string& str){  
    aes_types::ilist vct;
    // converts a single hex number in int
    auto hex_to_int = [](const std::string& s) -> int{
      int t = 0;
      for (int n = 0; n < s.length(); n++){
        int current;
        if (s[n] >= 'a' && s[n] <= 'f'){
          current = s[n] - 'a' + 10;
        }
        else if (s[n] >= '0' && s[n] <= '9'){
          current = s[n] - '0';
        }
        else {
          throw std::invalid_argument("Got invalid hex character: "+s[n]);
        }
        t += current * pow(16, s.length()-n-1);
      }
      return t;

    };
    for (int i = 0; i < str.length(); i += 2){
      vct.push_back(hex_to_int({str[i], str[i+1]}));
    }
    return vct;
    }
  aes_types::ilist basic_from_oct(const std::string& str){  
    aes_types::ilist vct;
    // converst a single oct digit in int
    auto oct_to_int = [](const std::string& s) -> int{
      int t = 0;
      for (int n = 0; n < s.length(); n++){
        int current = 0;
        if (s[n] >= '0' && s[n] <= '7'){
          current = s[n] - '0';
        }
        else {
          throw std::invalid_argument("Got invalid oct character: "+s[n]);
        }
        t += current * pow(8, s.length()-n-1);
      }
      return t;
    };
    for (int n = 0; n < str.length(); n += 3){
      vct.push_back(oct_to_int({str[n], str[n+1], str[n+2]}));
    }
    return vct;
  }
  
  // convert vector of uint in hex/oct string
  std::string basic_hex(aes_types::ilist bytes){  
    std::stringstream ss;
    
    for (int i : bytes) {
      ss << std::hex << std::setw(2) << std::setfill('0') << i;
    }

    return ss.str();
  }
  std::string basic_oct(aes_types::ilist bytes){  
    std::stringstream ss;
    
    for (int i : bytes) {
      ss << std::oct << std::setw(3) << std::setfill('0') << i;
    }

    return ss.str();
  }
  template <size_t len>
  std::string basic_hex(aes_types::iarr<len> bytes){
    aes_types::ilist vct;
    vct.assign(bytes.begin(), bytes.end());
    return aes_functions::basic_hex(vct);
  }
  template <size_t len>
  std::string basic_oct(aes_types::iarr<len> bytes){
    aes_types::ilist vct;
    vct.assign(bytes.begin(), bytes.end());
    return aes_functions::basic_oct(vct);
  }
} // namespace aes_functions
