#pragma once
#include <SFML/Audio.hpp>
#include <map>

class AudioManager{
private:
  static const int NUM_SOUNDS = 5;
  static std::map<std::string, sf::SoundBuffer*> soundBuffers;
  static std::map<std::string, std::string> musics;
  static sf::Sound sounds[NUM_SOUNDS];
  static sf::Music music;

  //load a sound into the soundBuffers with reference name key
  //and filename sound
  static void loadSound(std::string key, std::string sound);

public:
  //load all the sounds
  static void loadSounds();

  //play the given sound at given position
  static void playSound(std::string key, float x, float y, float z);

  //play this music
  static void playMusic(std::string music);

};
