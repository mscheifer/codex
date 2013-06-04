#include "ConfigManager.h"
#include <iostream>
#include <time.h>
#include "util.h"

  //convert the log level to it's string value
std::string ConfigManager::levelToString(LogLevels lev) {
  switch(lev) {
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

ConfigManager::configMap_t ConfigManager::configMap;
std::ofstream ConfigManager::logfile;
bool ConfigManager::read = false;

void ConfigManager::log(std::string str, ConfigManager::LogLevels lev){
  if( StringToNumber<int>(ConfigManager::configMap["log"]) == 0 ) 
    return;
  time_t timer;
  timer = time(nullptr);
  struct tm * currTime = localtime(&timer);
  
  if( ConfigManager::level <= lev){
    ConfigManager::logfile /*<< "[" << ConfigManager::levelToString(lev) << ":" <<
      currTime->tm_hour << ":" << currTime->tm_min << ":" << currTime->tm_sec <<
      "]"  */<< str << std::endl;
  }
}

void ConfigManager::setupLog(std::string str){
  if( StringToNumber<int>(ConfigManager::configMap["log"]) == 0 ) 
    return;

  time_t timer;
  timer = time(nullptr);
  struct tm * currTime = localtime(&timer);
  
  std::stringstream fname;
  fname << "logs/" << str << "LOG" << currTime->tm_mon+1 << "_" << currTime->tm_mday << "_" <<
    currTime->tm_hour << "_" << currTime->tm_min << "_" << currTime->tm_sec << ".txt";
  
  ConfigManager::logfile.open(fname.str());
}

void ConfigManager::readConfig() {
  if(read) {
    return;
  }

  read = true;
  std::ifstream configFile;
  configFile.open("config.txt");
  std::string line;

  //no config file exists
  if(!configFile){
    configFile.open("masterConfig.txt");
    std::cout << "no config detected, reading master" << std::endl;
  }

  size_t ind = 0;
  while( configFile >> line ){
    if(line.find_first_of("//",0) == 0) {
      continue;
    }
    ind = line.find('=');
    if( ind == std::string::npos ) {
      continue;
    }
    //std::cout << line << std::endl;
      
    //std::cout << "insert: " << line.substr(0,ind) << " = " << line.substr(ind+1) << std::endl;
    std::pair<std::string,std::string> s = std::make_pair(line.substr(0,ind),line.substr(ind+1));
    if(!ConfigManager::configMap.insert(s).second) {
      std::cout << "Error reading config file" << std::endl;
    //std::cout << "key " << line.substr(0,ind) << " = " << ConfigManager::configMap[line.substr(0,ind)] << std::endl;
    }
  }
}

float ConfigManager::playerMovescale() {
  if(!read) readConfig();
  return StringToNumber<float>(ConfigManager::configMap["movescale"]);
}
