#include <iostream>
#include <fstream>
#include <string>
#include <map>

class ConfigManager{
public:
  typedef std::map<std::string, std::string> configMap_t;
  static configMap_t configMap;
  static void readConfig();
};