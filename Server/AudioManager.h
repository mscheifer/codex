#pragma once
#include <SFML/Audio.hpp>
#include <map>
#include <array>

#include "Player.h"
#include "Wall.h"
#include "Projectile.h"
//#include "Weapon.h"
//#include "Powerup.h"

class AudioManager{
private:
  static std::map<std::string, sf::SoundBuffer*> soundBuffers;
  static std::map<std::string, std::string> musics;
  static std::list<sf::Sound> sounds;
  static std::array<sf::Music,4> music;
  static bool useSound;
  static int trackNo;
  static const int maxTracks = 2;

  //load a sound into the soundBuffers with reference name key
  //and filename sound
  static void loadSound(std::string key, std::string sound);
  static void playSoundHelper(std::list<sf::Sound>::iterator index, v3_t pos, sf::SoundBuffer* sbuff);

public:
  //initialize the audio manager
  //load all the sounds
  static void loadSounds();

  //play the given sound at given position
  //replace = try to replace the same sound
  //force = if no free sounds, kick one out
  static void playSound(std::string key, v3_t pos);

  //play this music
  //numPlayers is the players in close proximity
  static void updateMusic( int numPlayers );
  static void loadTrack( int i );
  static void playMusic(std::string music, int index);

  static void processPlayerSound(Player& o);
  static void processProjectileSound(Projectile& o);
};
