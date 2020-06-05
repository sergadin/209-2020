#include <istream>

namespace util {

namespace ios {
struct SkipSpaces {} skipws;
struct SkipCommas {} skipcm;
}  // namespace ios
}  // namespace util

std::istream &operator>>(std::istream &is, const util::ios::SkipSpaces &) {
  char c;
  while (true) {
    c = is.peek();
    if (c == '\r' || c == '\n' || c == '\t' || c == ' ')
      is.ignore();
    else
      return is;
  }
}

std::istream &operator>>(std::istream &is, const util::ios::SkipCommas &) {
  char c;
  c = is.peek();
  if (c == ',') is.ignore();
  return is;
}