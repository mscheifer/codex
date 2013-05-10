#include "ConfigManager.h"
#include <iostream>
#include <time.h>
#include "util.h"

  //convert the log level to it's string value
std::string ConfigManager::levelToString(LogLevels level) {
  switch(level) {
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

void ConfigManager::log(std::string str, ConfigManager::LogLevels level){
  if( StringToNumber<int>(ConfigManager::configMap["log"]) == 0 ) 
    return;

  time_t timer;
  timer = time(nullptr);
  struct tm * currTime = localtime(&timer);
  
  if( ConfigManager::level <= level ){
    ConfigManager::logfile << "[" << ConfigManager::levelToString(level) << ":" <<
      currTime->tm_hour << ":" << currTime->tm_min << ":" << currTime->tm_sec <<
      "]" << str << std::endl;
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
    ind = line.find('=');
    std::pair<std::string,std::string> s = std::pair<std::string,std::string>(line.substr(0,ind),line.substr(ind+1));
    ConfigManager::configMap.insert(s);
    //std::cout << line.substr(0,ind) << " = " << line.substr(ind+1) << std::endl;
  }
}