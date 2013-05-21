#ifndef UTIL_H
#define UTIL_H
#include <sstream>

template <typename T>
T StringToNumber (const std::string& Text) {
  std::istringstream ss(Text);
  T result;
  return ss >> result ? result : 0;
}

bool StringToBool(std::string);

std::string readFile(const std::string);
#endif
