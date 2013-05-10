#ifndef UTIL_H
#define UTIL_H
#include <sstream>
#include <algorithm>

template <typename T>
T StringToNumber (const std::string &Text) {
  std::istringstream ss(Text);
  T result;
  return ss >> result ? result : 0;
}

inline bool StringToBool(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    std::istringstream is(str);
    bool ret;
    is >> std::boolalpha >> ret;
    return ret;
}
#endif