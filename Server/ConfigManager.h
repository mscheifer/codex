#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <time.h>
#include <stdlib.h>
#include <sstream>
  
  template <typename T>
  T StringToNumber (const std::string &Text ){
    istringstream ss(Text);
    T result;
    return ss >> result ? result : 0;
  }

class ConfigManager{
public:

  enum LogLevels {DEBUG1=1, DEBUG2=2, NOTE=3};
  static const LogLevels level = ConfigManager::DEBUG2;
  static std::ofstream logfile;

  typedef std::map<std::string, std::string> configMap_t;
  static configMap_t configMap;
  //read and initialize the config file
  static void readConfig();
  
  //log str with given log level
  static void log( std::string str, LogLevels l);
  
  //setup the logfile
  static void setupLog();

  //convert the log level to it's string value
  static std::string levelToString(LogLevels level){
    switch(level){
    case DEBUG1:
      return "DEBUG1";
    case DEBUG2:
      return "DEBUG2";
    case NOTE:
      return "NOTE";
    default:
      return "UNK";
    }
  }
};