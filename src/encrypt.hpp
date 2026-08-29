#pragma once

#include "crypt/add_round_key.hpp"
#include "crypt/mix_columns.hpp"
#include "crypt/shift_rows.hpp"
#include "crypt/sub_bytes.hpp"
#include "message.hpp"
#include "key.hpp"

Message encrypt_aes(Message message, const Key& key){
  if (! key.is_expanded()){
    throw std::runtime_error("Trying to crypt a message with an unexpanded key, expand it by call key.expand()");
  }

  size_t rounds = key.get_specs().total_rounds;
  for (size_t round = 0; round <= rounds; round++){
    if (round > 0){
      message = crypt_operations::sub_bytes(message);
      message = crypt_operations::shift_rows(message);
      if (round < rounds){
        message = crypt_operations::mix_columns(message);
      }
    }
    message = crypt_operations::add_round_key(message, key, round);
  }

  return message;
}

Message decrypt_aes(Message encrypted, const Key& key){
  if (! key.is_expanded()){
    throw std::runtime_error("Trying to decrypt an encrypted message with an unexpanded key, expand it by call key.expand()");
  }

  size_t rounds = key.get_specs().total_rounds;
  size_t round = rounds+1;
  while (round --> 0){
    if (round < rounds){
        encrypted = crypt_operations::inv_shift_rows(encrypted);
        encrypted = crypt_operations::inv_sub_bytes(encrypted);
    }
    encrypted = crypt_operations::add_round_key(encrypted, key, round);
    if (round < rounds && round > 0){
      encrypted = crypt_operations::inv_mix_columns(encrypted);
    }
  }
  return encrypted;
}