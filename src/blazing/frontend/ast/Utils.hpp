#pragma once

#include <stdexcept>
#include <string>
#include <string_view>

namespace blazing {

inline std::string escapeString(std::string_view v) {
  std::string decoded_string = "";
  for (int i = 1; i < v.size() - 1; ++i) {
    if (v[i] == '\\') {
      switch (v[i + 1]) {
      case '"':
        decoded_string += '"';
        break;
      case 'r':
        decoded_string += '\r';
        break;
      case 't':
        decoded_string += '\t';
        break;
      case 'n':
        decoded_string += '\n';
        break;
      default:
        throw std::runtime_error(std::string("Unknown escape sequence \\") + v[i + 1]);
        break;
      }
      ++i;
    } else {
      decoded_string += v[i];
    }
  }
  return decoded_string;
}

} // namespace blazing
