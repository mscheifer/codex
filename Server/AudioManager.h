#pragma once
#include <SFML/Audio.hpp>
#include <map>
#include "Entity.h"
#include "Player.h"
#include "Wall.h"
#include "Projectile.h"
#include "Weapon.h"
#include "PowerUp.h"
#include <array>

class AudioManager{
private:
  static const int NUM_SOUNDS = 5;
  static std::map<std::string, sf::SoundBuffer*> soundBuffers;
  static std::map<std::string, std::string> musics;
  static std::array<sf::Sound, NUM_SOUNDS> sounds;
  static std::array<int, NUM_SOUNDS> soundOwner; //player id, -1 is other(wall etc)
  static sf::Music music;
  static bool useSound;

  //load a sound into the soundBuffers with reference name key
  //and filename sound
  static void loadSound(std::string key, std::string sound);

public:
  //initialize the audio manager
  //load all the sounds
  static void loadSounds();

  //play the given sound at given position
  static void playSound(std::string key, int id, 
    float x, float y, float z, bool force = false);

  //play this music
  static void playMusic(std::string music);

  static void processEntitySound(Player& o);
  static void processEntitySound(Wall& o);
  static void processEntitySound(Projectile& o);
  static void processEntitySound(PowerUp& o);
  static void processEntitySound(Weapon& o);
};
