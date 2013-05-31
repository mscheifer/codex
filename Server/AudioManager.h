#pragma once
#include <SFML/Audio.hpp>
#include <map>
#include <array>

#include "Player.h"
#include "Wall.h"
#include "Projectile.h"
#include <array>

class AudioManager{
private:
  static std::map<std::string, sf::SoundBuffer*> soundBuffers;
  static std::map<std::string, std::string> musics;
  static std::map<std::string, sf::Sound> sounds;
  static std::map<std::string, sf::Sound> playerSounds[4];
  static std::array<sf::Music,2> music;
  static std::array<int,2> musicProx;
  static bool useSound;
  static int trackNo;
  static const int maxTracks = 2;
  static unsigned int currentlyPlayingMusic;

  //load a sound into the soundBuffers with reference name key
  //and filename sound
  static void loadSound(std::string key, std::string sound);
  static void playSoundHelper(sf::Sound* s, v3_t pos, sf::SoundBuffer* sbuff);

  static std::string getWeaponCall(WeaponType w);

  //this is for music
  static int notCurrentlyPlaying(); //return the index of the not currently playing music
  static std::string getTrack( int track, int prox ); // get the string of music based on track and prox
  static void loadTrack( int i ); //load the track
  static void playMusic(std::string music, int index); //helper which playes music onto given index
  static int getClosestProx(int prox); //return the next closest proximity to load in teh other music (this is done on init only)

public:
  static float soundScaling;
  //initialize the audio manager
  //load all the sounds
  static void loadSounds();

  //play the given sound at given position
  //replace = try to replace the same sound
  //force = if no free sounds, kick one out
  static void playSound(std::string key, std::string id,  v3_t pos);
  
  static void playPlayerSound(std::string sound, int player_id , std::string name, v3_t p);
  static void stopSound(std::string id);
   static void stopPlayerSound(int id , std::string key);
  //play this music
  //numPlayers is the players in close proximity
  static void updateMusic( int numPlayers ); //main method that updates which music to play

  static void processPlayerSound(Player& o);
  static void processProjectileSound(Projectile& o);
};
