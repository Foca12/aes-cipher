#pragma once

#include "helpers.hpp"
#include "state.hpp"

class Message {
  aes_types::message_vct states;

  // helper functions that calculate negative indexes and check for indixes exceeding the maximum length
  size_t handle_state_idx(const int idx) const {
    size_t result = idx < 0? this->states.size() + idx : idx;
    if (result >= this->states.size()){
      throw std::out_of_range("Message state idx is exceeded maximum length");
    }
    return result;
  }
  size_t handle_idx(const int idx) const {
    size_t result = idx < 0? this->states.size() * aes_constants::state_chars + idx : idx;
    if (result >= this->states.size() * aes_constants::state_chars){
      throw std::out_of_range("Message state idx is exceeded maximum length");
    }
    return result;
  }

  struct support_iterator {
    aes_types::message_vct support_states;

    aes_types::message_iterator begin(){
      return this->support_states.begin();
    }
    aes_types::message_iterator end(){
      return this->support_states.end();
    }
  };

  public:
  ~Message() {
    // clear memory 
    this->clear();
  }
  
  Message(){}
  Message(const aes_types::message_vct& list){
    this->states = list;
  }

  static Message divide_string(const std::string& bytes){
    aes_types::ilist vct;
    for (auto i : bytes){
      vct.push_back(static_cast<uint8_t> (i));
    }
    return Message::divide_ilist(vct);
  }
  static Message divide_ilist(aes_types::ilist bytes){
    Message message;
    State current;

    // number of zeros to add ot bytes for make it a multiple of 16
    size_t padding = bytes.size() % aes_constants::state_chars;
    // number of states
    size_t states = bytes.size() / aes_constants::state_chars;
    if (padding > 0) {
      states ++;
      padding = aes_constants::state_chars - padding;
    }

    bytes.insert(bytes.end(), padding, 0);

    for (size_t state_idx = 0; state_idx < states; state_idx++){
      current.clear();
      for (size_t char_idx = 0; char_idx < aes_constants::state_chars; char_idx++){
        size_t state_start = state_idx*aes_constants::state_chars;
        current[char_idx] = bytes[char_idx+state_start];
      }
      message.extend(current);
    }
    return message;
  }

  void clear() {
    for (State& current_state : this->states){
      current_state.clear();
    }
  }

  int length() const {
    return this->states.size();
  }
  int size() const {
    return this->length();
  }
  
  void extend(const State& bytes) {
    this->states.push_back(bytes);
  }
  void push_back(const State& bytes) {
    this->states.push_back(bytes);
  }

  uint8_t& operator[](int id) {
    size_t idx = this->handle_idx(id);
    size_t state_idx = idx / aes_constants::state_chars;
    size_t element_idx = idx % aes_constants::state_chars;

    return this->states[state_idx][element_idx];
  }
  uint8_t operator[](int id) const {
    size_t idx = this->handle_idx(id);
    size_t state_idx = idx / aes_constants::state_chars;
    size_t element_idx = idx % aes_constants::state_chars;

    return this->states[state_idx][element_idx];
  }
  State& state(int idx){
    size_t state_idx = this->handle_state_idx(idx);
    return this->states[state_idx];
  }
  State state(int idx) const {
    size_t state_idx = this->handle_state_idx(idx);
    return this->states[state_idx];
  }

  typename aes_types::iarr_c_iterator<aes_constants::state_chars> begin() const {
    return this->states[0].begin();
  }
  typename aes_types::iarr_c_iterator<aes_constants::state_chars> end() const {
    return this->states[0].end();
  }
  
  support_iterator state_iterator() const {
    return support_iterator{this->states};
  }

  Message& operator=(const Message& message){
    this->states = message.states;
    return *this;
  }

  Message operator^(const State& state) const {
    Message result;
    for (State i : this->state_iterator()){
      result.extend(state ^ i);
    }
    return result;
  }
  Message operator&(const State& state) const {
    Message result;
    for (State i : this->state_iterator()){
      result.extend(state & i);
    }
    return result;
  }
  Message operator|(const State& state) const {
    Message result;
    for (State i : this->state_iterator()){
      result.extend(state | i);
    }
    return result;
  }

  Message& operator^=(const State& state) {
    return this->operator=(this->operator^(state));
  }
  Message& operator&=(const State& state) {
    return this->operator=(this->operator&(state));
  }
  Message& operator|=(const State& state) {
    return this->operator=(this->operator|(state));
  }

  Message operator>> (const int rounds) const {
    Message result;
    for (auto i : this->state_iterator()){
      result.extend(i >> rounds);
    }
    return result;
  }
  Message operator<< (const int rounds) const {
    Message result;
    for (auto i : this->state_iterator()){
      result.extend(i << rounds);
    }
    return result;
  }

  Message& operator>>=(const int rounds) {
    return this->operator=(this->operator>>(rounds));
  }
  Message& operator<<=(const int rounds) {
    return this->operator=(this->operator<<(rounds));
  }

  Message shift_left(int rounds) const {
    Message result;
    for (State i : this->state_iterator()){
      result.extend(i.shift_left(rounds));
    }
    return result;
  }
  Message shift_right(int rounds) const {
    Message result;
    for (State i : this->state_iterator()){
      result.extend(i.shift_right(rounds));
    }
    return result;
  }

  // cast di tipi
  operator std::string() const {
    std::string str = "";
    for (auto i : this->state_iterator()){
      str += (std::string)i;
    }
    return str;
  }
  operator aes_types::ilist() const {
    aes_types::ilist list = {};
    for (State i : this->states){
      list.insert(list.end(), static_cast<aes_types::ilist>(i).begin(), static_cast<aes_types::ilist>(i).end());
    }
    return list;
  }

  // conversioni di formato
  std::string hex() const {
    std::string str = "";
    
    for (State i : this->states) {
      str += i.hex();
    }

    return str;
  }
  std::string oct() const {
    std::string str = "";
    
    for (State i : this->states) {
      str += i.oct();
    }

    return str;
  }

  // costruttori alternativi
  static Message from_hex(const std::string& str){
    aes_types::ilist array = aes_functions::basic_from_hex(str);
    return Message::divide_ilist(array);
  }
  static Message from_oct(const std::string& str){
    aes_types::ilist array = aes_functions::basic_from_oct(str);
    return Message::divide_ilist(array);
  }
};