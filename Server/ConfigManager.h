#pragma once
#include <fstream>
#include <string>
#include <map>
#include "util.h"

class ConfigManager {
  enum LogLevels {DEBUG1=1, DEBUG2=2, NOTE=3}; //in order of decreasing scope (lower will show more)
  static const LogLevels level = ConfigManager::DEBUG1; //Change this to access more/less
  static std::ofstream logfile;

  static std::string levelToString(LogLevels);

  typedef std::map<std::string, std::string> configMap_t;

public:
  static configMap_t configMap;
  //read and initialize the config file
  static void readConfig();

  static int numPlayers(){
    return StringToNumber<int>(ConfigManager::configMap["players"]);
  }
  
  //log str with given log level
  static void log( std::string str, LogLevels l=DEBUG1);
  
  //setup the logfile
  static void setupLog(std::string str = "");
};
