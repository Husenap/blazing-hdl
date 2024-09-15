#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>

#include "SourceLoc.hpp"

namespace blazing {

class SyntaxError : public std::runtime_error {
  static std::string makeMessage(location loc, std::string_view reason) {
    std::stringstream ss;
    ss << loc << ": " << reason << std::endl;

    if (loc.begin.filename) {
      std::ifstream file(*loc.begin.filename);
      int           l = loc.begin.line;
      std::string   line;
      while (l--) {
        std::getline(file, line);
      }
      ss << line << std::endl;
      int x0 = loc.begin.column;
      int x1 = loc.end.column;
      for (int i = 0; i < x1; ++i) {
        if (i < x0)
          ss << ' ';
        else if (i == x0)
          ss << '^';
        else
          ss << '~';
      }
      ss << " here" << std::endl;
    }

    return ss.str();
  }

public:
  SyntaxError(location loc, std::string_view reason) : runtime_error(makeMessage(loc, reason)) {}
};

} // namespace blazing
