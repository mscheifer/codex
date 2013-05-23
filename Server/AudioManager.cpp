#include "AudioManager.h"
#include "ConfigManager.h"
#include "util.h"

std::array<sf::Music,4> AudioManager::music;
std::map<std::string, sf::SoundBuffer*> AudioManager::soundBuffers;
std::map<std::string, std::string> AudioManager::musics;
std::list<sf::Sound> AudioManager::sounds;
bool AudioManager::useSound;
int AudioManager::trackNo;

void AudioManager::loadSound(std::string key, std::string sound){
  if(useSound) 
    return;

  sf::SoundBuffer *buffer = new sf::SoundBuffer;
  buffer->loadFromFile(sound);
  soundBuffers.insert(std::pair<std::string,sf::SoundBuffer*>
    (key,buffer));
}

void AudioManager::loadSounds(){
  useSound = StringToNumber<int>(ConfigManager::configMap["sound"]) == 0;
  if(useSound)
    return;
  
  trackNo = 0;
  loadSound("s1", "sounds/sound_1.wav");
  loadSound("s2", "sounds/sound_2.wav");
  loadSound("s3", "sounds/sound_3.wav");
  loadSound("m1", "sounds/music_mono.wav");
  loadSound("f1", "sounds/105016__julien-matthey__jm-fx-fireball-01.wav");
  //loadSound("s5", "sound_5.wav");

  musics["m1"] = "sounds/music.wav";

  //track 0
  musics["m1_1"] = "sounds/m1_1.wav";
  musics["m1_2"] = "sounds/m1_2.wav";
  musics["m1_3"] = "sounds/m1_3.wav";
  musics["m1_4"] = "sounds/m1_4.aif";

  //track 1
  musics["m2_1"] = "sounds/m2_1.wav";
  musics["m2_2"] = "sounds/m2_2.wav";
  musics["m2_3"] = "sounds/m2_3.wav";
  musics["m2_4"] = "sounds/m2_4.aif";
}

void AudioManager::playSound(std::string key, v3_t pos){
  if(useSound) 
    return;

  std::map<std::string,sf::SoundBuffer*>::iterator it;
  it = soundBuffers.find(key);

  if(it != soundBuffers.end()){ //element exists
    auto sound = sounds.begin();
    for(sound = sounds.begin(); sound != sounds.end(); sound++){
      if(sound->getStatus() != sf::Sound::Playing){
        playSoundHelper(sound,pos,it->second);
        break;
      }
    }

    if(sound == sounds.end()){
      sounds.push_front(sf::Sound());
      playSoundHelper(sounds.begin(),pos,it->second);
    }
  }
}

void AudioManager::playSoundHelper(std::list<sf::Sound>::iterator index, v3_t pos, sf::SoundBuffer* sbuff){
  index->setBuffer(*sbuff);
  index->setPosition(pos.x,pos.y,pos.z);
  index->play();
}

void AudioManager::updateMusic( int numPlayers ){

  //switch tracks
  if(music[0].getStatus() != sf::Sound::Playing){
    trackNo = ++trackNo % maxTracks; //this shit is undefined, fix it
    loadTrack(trackNo);

    //set the track to currently play
    music[numPlayers].setVolume(100);
  }

  //update the volumes
  float volume = 0;
  for( unsigned int i = 0; i < music.size(); i++ ){
    if( i == numPlayers ){ //FADE IN
      volume = music[i].getVolume();
      volume += 5;
      if(volume > 100)
        volume = 100;
      music[i].setVolume(volume);
    }
    else{ //FADE OUT
      volume = music[i].getVolume();
      volume -= 5;
      if(volume < 0)
        volume = 0;
      music[i].setVolume(volume);
    }
  }
  
}

void AudioManager::loadTrack(int i){
  if( i == 0 ){
    playMusic("m1_1", 0);
    playMusic("m1_2", 1);
    playMusic("m1_3", 2);
    playMusic("m1_4", 3);
  } else if( i == 1 ){
    playMusic("m2_1", 0);
    playMusic("m2_2", 1);
    playMusic("m2_3", 2);
    playMusic("m2_4", 3);
  }

  for(auto itr = music.begin(); itr != music.end(); itr++) {
    itr->setVolume(0);
  }
}

void AudioManager::playMusic(std::string musicN, int index){
  if(useSound) 
    return;

  if (music[index].openFromFile(musics[musicN])){
    music[index].play();
  }
}

void AudioManager::processPlayerSound(Player& o){


}

void AudioManager::processProjectileSound(Projectile& o){
  if(o.getFired()){
    playSound("f1", o.getPosition());
  }
}

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
