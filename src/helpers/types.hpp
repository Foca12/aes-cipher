#pragma once

#include "constants.hpp"

class State;

namespace aes_types
{
  // general types
  template <size_t len>
  using iarr = std::array<uint8_t, len>;
  using ilist = std::vector<uint8_t>;

  template <size_t len>
  using iarr_c_iterator = typename std::array<uint8_t, len>::const_iterator;

  // state types
  using state_arr = std::array<uint8_t, aes_constants::state_chars>;
  using state_row = std::array<uint8_t, aes_constants::state_columns>;
  using state_column = std::array<uint8_t, aes_constants::state_rows>;
  using state_matrix_row = std::array<aes_types::state_row, aes_constants::state_rows>;
  using state_matrix_column = std::array<aes_types::state_column, aes_constants::state_columns>;


  // message types
  using message_vct = std::vector<State>;
  using message_iterator = std::vector<State>::iterator;
} // namespace aes_types
