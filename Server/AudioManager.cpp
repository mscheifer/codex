#include "AudioManager.h"
#include "ConfigManager.h"

sf::Music AudioManager::music;
sf::Sound AudioManager::sounds[NUM_SOUNDS];
std::map<std::string, sf::SoundBuffer*> AudioManager::soundBuffers;
std::map<std::string, std::string> AudioManager::musics;

/*
  int x = 0;
  sf::Listener::setPosition(x, 0.f, 0.f);
  sf::Listener::setDirection(1.f, 0.f, 0.f);
  AudioManager::loadSounds();
  AudioManager::playMusic("m1");
  
  //play multiple sound tests
  sf::Clock clock;
  clock.restart();
  while(true){
    if( clock.getElapsedTime().asSeconds() > 3 ){
      std::cout << "play s1" << std::endl;
      break;
    }
  }
  AudioManager::playSound("s1",10,0,0);
  while(true){
    if( clock.getElapsedTime().asSeconds() > 6 ){
      std::cout << "play s2" << std::endl;
      break;
    }
  }
  AudioManager::playSound("s2", 0,0,0);
  while(true){
    if( clock.getElapsedTime().asSeconds() > 9 ){
      std::cout << "play s1" << std::endl;
      break;
    }
  }
  AudioManager::playSound("s1",-2,0,0);
  while(true){
  }

  // 3d sound test
  sf::SoundBuffer buffer;
  buffer.loadFromFile("musicMono.wav");
  sf::Sound sound;
  sound.setBuffer(buffer);
  sound.setPosition(100.f, 0.f, -5.f);
  sound.play();

  sf::Clock clock;
  clock.restart();
  while(true){
    if( clock.getElapsedTime().asMilliseconds() > 1000 ){
        clock.restart();
        sf::Listener::setPosition(x+=10, 0.f, 0.f);
        std::cout << x << " ";
    }
  }//end 3d sound test
*/

void AudioManager::loadSound(std::string key, std::string sound){
  if( StringToNumber<int>(ConfigManager::configMap["sound"]) == 0 ) 
    return;

  sf::SoundBuffer *buffer = new sf::SoundBuffer;
  buffer->loadFromFile(sound);
  soundBuffers.insert(std::pair<std::string,sf::SoundBuffer*>
    (key,buffer));
}

void AudioManager::loadSounds(){
  if( StringToNumber<int>(ConfigManager::configMap["sound"]) == 0 ) 
    return;

  loadSound("s1", "sound_1.wav");
  loadSound("s2", "sound_2.wav");
  loadSound("s3", "sound_3.wav");
    loadSound("m1", "music_mono.wav");
  //loadSound("s4", "sound_4.wav");
  //loadSound("s5", "sound_5.wav");

  musics["m1"] = "music.wav";
}

void AudioManager::playSound(std::string key, float x, float y, float z){
  if( StringToNumber<int>(ConfigManager::configMap["sound"]) == 0 ) 
    return;
  std::map<std::string,sf::SoundBuffer*>::iterator it;
  it = soundBuffers.find(key);

  if(it != soundBuffers.end()){ //element exists
    for(int i = 0; i < NUM_SOUNDS; i++){
      if(sounds[i].getStatus() != sf::Sound::Playing){
        sounds[i].setBuffer(*((*it).second));
        sounds[i].setPosition(x,y,z);
        sounds[i].play();
        break;
      }
    }
  }
}

void AudioManager::playMusic(std::string musicN){
  if( StringToNumber<int>(ConfigManager::configMap["sound"]) == 0 ) 
    return;
  if (music.openFromFile(musics[musicN]))
  {
    music.play();
  }
}