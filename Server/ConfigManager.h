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
    static float ret = 1000.f / static_cast<float>(serverTicksPerSec());
    return ret;
  }
  
  static float serverTickLengthSec() {
    static float ret = 1.f / static_cast<float>(serverTicksPerSec());
    return ret;
  }

  //log str with given log level
  static void log( std::string str, LogLevels l=DEBUG1);
  
  //setup the logfile
  static void setupLog(std::string str = "");

  //graphics
  static float fullscreen(){ return StringToBool(ConfigManager::configMap["fullscreen"]); }
  static unsigned int antiAliasingLevel(){ return StringToNumber<unsigned int>(ConfigManager::configMap["anti-alias"]); }
  static unsigned int windowWidth(){return StringToNumber<unsigned int>(ConfigManager::configMap["window-width"]); }
  static unsigned int windowHeight(){return StringToNumber<unsigned int>(ConfigManager::configMap["window-height"]); }

  static float musicProx(){ return StringToNumber<float>(ConfigManager::configMap["musicProx"]); }

  /// Player/Minotaur Related///
  static float playerMovescale();
  static float playerAirMovescale(){ return StringToNumber<float>(ConfigManager::configMap["airMovescale"]); }
  static float playerJumpSpeed(){ return StringToNumber<float>(ConfigManager::configMap["jumpSpeed"]); }
  static int playerMaxJump(){ return StringToNumber<int>(ConfigManager::configMap["maxJump"]); }
  static float playerHpRegen(){ return StringToNumber<float>(ConfigManager::configMap["playerHpRegen"]); }
  static float playerMpRegen(){ return StringToNumber<float>(ConfigManager::configMap["playerMpRegen"]); }
  static float playerDef(){ return StringToNumber<float>(ConfigManager::configMap["playerDef"]); }
  static float playerHp(){ return StringToNumber<float>(ConfigManager::configMap["playerHp"]); }
  static float playerMaxHp(){ return StringToNumber<float>(ConfigManager::configMap["playerMaxHp"]); }
  static float playerMp(){ return StringToNumber<float>(ConfigManager::configMap["playerMp"]); }
  static float playerMaxMp(){ return StringToNumber<float>(ConfigManager::configMap["playerMaxMp"]); }
  
  static float minotaurMovescale(){ return StringToNumber<float>(ConfigManager::configMap["minotaurmovescale"]); }
  static float minotaurAirMovescale(){ return StringToNumber<float>(ConfigManager::configMap["minotaurairMovescale"]); }
  static float minotaurJumpSpeed(){ return StringToNumber<float>(ConfigManager::configMap["minotaurjumpSpeed"]); }
  static int minotaurMaxJump(){ return StringToNumber<int>(ConfigManager::configMap["minotaurmaxJump"]); }
  static float minotaurHpRegen(){ return StringToNumber<float>(ConfigManager::configMap["minotaurHpRegen"]); }
  static float minotaurMpRegen(){ return StringToNumber<float>(ConfigManager::configMap["minotaurMpRegen"]); }
  static float minotaurDef(){ return StringToNumber<float>(ConfigManager::configMap["minotaurDef"]); }
  static float minotaurHp(){ return StringToNumber<float>(ConfigManager::configMap["minotaurHp"]); }
  static float minotaurMaxHp(){ return StringToNumber<float>(ConfigManager::configMap["minotaurMaxHp"]); }
  static float minotaurMp(){ return StringToNumber<float>(ConfigManager::configMap["minotaurMp"]); }
  static float minotaurMaxMp(){ return StringToNumber<float>(ConfigManager::configMap["minotaurMaxMp"]); }
  static bool gameRestart(){ return StringToNumber<bool>(ConfigManager::configMap["gameRestart"]); }
  
  /// Projectile ///
  static float projectileRange(){ return StringToNumber<float>(ConfigManager::configMap["projectileRange"]); }

  /// Wall ///
  static float wallDepth(){ return StringToNumber<float>(ConfigManager::configMap["wallDepth"]); }
  static float wallHeight(){ return StringToNumber<float>(ConfigManager::configMap["wallHeight"]); }
  static float wallWidth(){ return StringToNumber<float>(ConfigManager::configMap["wallWidth"]); }

  /// Physics ///
  static float gravity(){  return StringToNumber<float>(ConfigManager::configMap["gravity"]); }
};
