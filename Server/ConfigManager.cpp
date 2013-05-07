#include "ConfigManager.h"

ConfigManager::configMap_t ConfigManager::configMap;
std::ofstream ConfigManager::logfile;

void ConfigManager::log(std::string str, ConfigManager::LogLevels level){
  time_t timer;
  timer = time(NULL);
  struct tm * currTime = localtime(&timer);
  
  if( ConfigManager::level <= level ){
    ConfigManager::logfile << "[" << ConfigManager::levelToString(level) << ":" <<
      currTime->tm_hour << ":" << currTime->tm_min << ":" << currTime->tm_sec <<
      "]" << str << std::endl;
  }
}

void ConfigManager::setupLog(std::string str){
  time_t timer;
  timer = time(NULL);
  struct tm * currTime = localtime(&timer);
  
  std::stringstream fname;
  fname << str << "LOG" << currTime->tm_mon+1 << "_" << currTime->tm_mday << "_" <<
    currTime->tm_hour << "_" << currTime->tm_min << "_" << currTime->tm_sec << ".txt";
  
  ConfigManager::logfile.open(fname.str());

  //std::string fname = "" + currTime.tm_mon + "_" + currTime.tm_mday + currTime.tm_hour + currTime.tm_min + currTime.tm_sec;
}

void ConfigManager::readConfig(){
  std::ifstream configFile;
  configFile.open("config.txt");
  std::string line;

  //no config file exists
  if(!configFile){
    std::ofstream newConfigFile("config.txt");
    configFile.open("masterConfig.txt");
    while( configFile >> line ){
      newConfigFile << line << std::endl;
    }
    configFile.close();
    newConfigFile.close();
    configFile.clear();
    configFile.open("config.txt");
      
    std::cout << "no config file exists, generating" << std::endl;
  }

  size_t ind = 0;
  while( configFile >> line ){
    ind = line.find('=');
    std::pair<std::string,std::string> s = std::pair<std::string,std::string>(line.substr(0,ind),line.substr(ind+1));
    ConfigManager::configMap.insert(s);
    //std::cout << line.substr(0,ind) << " = " << line.substr(ind+1) << std::endl;
  }
  //std::cout << "a" << configMap["a"] << std::endl;
  //std::cout << "j" << configMap["j"] << std::endl;
  //std::cout << "x" << configMap["x"] << std::endl;
}