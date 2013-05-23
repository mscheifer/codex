#pragma once
#include <fstream>
#include <string>
#include <map>
#include "util.h"

class ConfigManager {
  enum LogLevels {DEBUG1=1, DEBUG2=2, NOTE=3}; //in order of decreasing scope (lower will show more)
  static const LogLevels level = ConfigManager::DEBUG1; //Change this to access more/less
  static std::ofstream logfile;
  static bool read;
  static std::string levelToString(LogLevels);
  typedef std::map<std::string, std::string> configMap_t;

public:
  static configMap_t configMap;
  //read and initialize the config file
  static void readConfig();

  static unsigned int numPlayers() {
    return StringToNumber<unsigned int>(ConfigManager::configMap["players"]);
  }

  static unsigned int serverTicksPerSec() {
    return StringToNumber<unsigned int>(ConfigManager::configMap["serverTicksPerSec"]);
  }

  static float serverTickLengthMilli() {
    static float ret = 1000.f / serverTicksPerSec();
    return ret;
  }
  
  static float serverTickLengthSec() {
    static float ret = serverTickLengthMilli() / 1000.f;
    return ret;
  }

  //log str with given log level
  static void log( std::string str, LogLevels l=DEBUG1);
  
  //setup the logfile
  static void setupLog(std::string str = "");

  /// Player/Minotaur Related///
  static const float playerMovescale(){ if(!read) readConfig(); return StringToNumber<float>(ConfigManager::configMap["movescale"]); }
  static const float playerAirMovescale(){ return StringToNumber<float>(ConfigManager::configMap["airMovescale"]); }
  static const float playerJumpSpeed(){ return StringToNumber<float>(ConfigManager::configMap["jumpSpeed"]); }
  static const int playerMaxJump(){ return StringToNumber<int>(ConfigManager::configMap["maxJump"]); }
  static const float playerHpRegen(){ return StringToNumber<float>(ConfigManager::configMap["playerHpRegen"]); }
  static const float playerMpRegen(){ return StringToNumber<float>(ConfigManager::configMap["playerMpRegen"]); }
  static const float playerDef(){ return StringToNumber<float>(ConfigManager::configMap["playerDef"]); }
  static const float playerHp(){ return StringToNumber<float>(ConfigManager::configMap["playerHp"]); }
  static const float playerMaxHp(){ return StringToNumber<float>(ConfigManager::configMap["playerMaxHp"]); }
  static const float playerMp(){ return StringToNumber<float>(ConfigManager::configMap["playerMp"]); }
  static const float playerMaxMp(){ return StringToNumber<float>(ConfigManager::configMap["playerMaxMp"]); }
  static const float minotaurHpRegen(){ return StringToNumber<float>(ConfigManager::configMap["minotaurHpRegen"]); }
  static const float minotaurMpRegen(){ return StringToNumber<float>(ConfigManager::configMap["minotaurMpRegen"]); }
  static const float minotaurDef(){ return StringToNumber<float>(ConfigManager::configMap["minotaurDef"]); }
  static const float minotaurHp(){ return StringToNumber<float>(ConfigManager::configMap["minotaurHp"]); }
  static const float minotaurMaxHp(){ return StringToNumber<float>(ConfigManager::configMap["minotaurMaxHp"]); }
  static const float minotaurMp(){ return StringToNumber<float>(ConfigManager::configMap["minotaurMp"]); }
  static const float minotaurMaxMp(){ return StringToNumber<float>(ConfigManager::configMap["minotaurMaxMp"]); }

  /// Projectile ///
  static const float projectileRange(){ return StringToNumber<float>(ConfigManager::configMap["projectileRange"]); }

  /// Physics ///
  static const float gravity(){  return StringToNumber<float>(ConfigManager::configMap["gravity"]); }
};
