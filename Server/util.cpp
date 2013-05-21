#include "util.h"
#include <iostream>
#include <fstream>
#include <algorithm>

bool StringToBool(std::string str) {
  std::transform(str.begin(), str.end(), str.begin(), ::tolower);
  std::istringstream is(str);
  bool ret;
  is >> std::boolalpha >> ret;
  return ret;
}

std::string readFile(const std::string fileName) {
  std::ifstream vsFile(fileName);
  std::string fullSource = "";

  if(vsFile.is_open()) {
    std::string line;
    while(vsFile.good()) {
      getline(vsFile,line);
      fullSource += line + "\n";
    }
    vsFile.close();
  } else {
    std::cout << "error: cannot open file: " << fileName << std::endl;
  }
  return fullSource;
}
