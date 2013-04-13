#include "ConfigManager.h"

ConfigManager::configMap_t ConfigManager::configMap;

void ConfigManager::readConfig(){
  std::ifstream configFile;
  configFile.open("config.txt");
  std::string line;

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