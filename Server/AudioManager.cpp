#include "AudioManager.h"
#include "ConfigManager.h"
#include "util.h"

std::array<sf::Music,2> AudioManager::music;
std::array<int,2> AudioManager::musicProx;
std::map<std::string, sf::SoundBuffer*> AudioManager::soundBuffers;
std::map<std::string, std::string> AudioManager::musics;
std::map<std::string, sf::Sound> AudioManager::sounds;
bool AudioManager::useSound;
int AudioManager::trackNo;
unsigned int AudioManager::currentlyPlayingMusic;

void AudioManager::loadSound(std::string key, std::string sound){
  if(!useSound) 
    return;

  sf::SoundBuffer *buffer = new sf::SoundBuffer;
  buffer->loadFromFile(sound);
  soundBuffers.insert(std::pair<std::string,sf::SoundBuffer*>
    (key,buffer));
}

void AudioManager::loadSounds(){
  useSound = StringToNumber<int>(ConfigManager::configMap["sound"]) == 1;
  if(!useSound)
    return;
  
  trackNo = 0;
  currentlyPlayingMusic = 0;
  for(unsigned int i = 0; i < musicProx.size(); i++){
    musicProx[i] = -1;
  }

  
  loadSound("c1", "sounds/charge1.wav");
  

  loadSound("w1", "sounds/foot_step_1.wav");
  loadSound("w2", "sounds/foot_step_2.wav");

  loadSound("s1", "sounds/sound_1.wav");
  loadSound("s2", "sounds/sound_2.wav");
  loadSound("s3", "sounds/sound_3.wav");
  loadSound("m1", "sounds/music_mono.wav");
  loadSound("f1", "sounds/m2_2.wav");
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

void AudioManager::playSound(std::string key, std::string id, v3_t pos){
  if(!useSound) 
    return;

  std::map<std::string,sf::SoundBuffer*>::iterator it;
  it = soundBuffers.find(key);

  if(it != soundBuffers.end()){ //element exists


    if( sounds.find(id) ==  sounds.end()) {
      sounds.insert(std::pair<std::string,sf::Sound>(id,sf::Sound()));
    }

    playSoundHelper( &sounds.find(id)->second ,pos, it->second);
  
  }
}


void AudioManager::stopSound(std::string id) {
   if( sounds.find(id) !=  sounds.end()) {
     sounds.find(id)->second.stop();
     sounds.erase(id);
   }
}

void AudioManager::playSoundHelper( sf::Sound* s, v3_t pos, sf::SoundBuffer* sbuff){
  s->setPosition(pos.x,pos.y,pos.z);
  if(s->getStatus() != sf::Sound::Playing)  {
    s->setBuffer(*sbuff);
    s->play();
  }

}

void AudioManager::updateMusic( int numPlayers ){
  return;
  if(!useSound)
    return;

  //switch tracks
  if(music[0].getStatus() != sf::Sound::Playing){
    ++trackNo;
    trackNo = trackNo % maxTracks;
    playMusic( getTrack(trackNo, numPlayers), currentlyPlayingMusic );
    playMusic( getTrack(trackNo, getClosestProx(numPlayers)), notCurrentlyPlaying() );

    //set the track to currently play
    music[currentlyPlayingMusic].setVolume(100);
    music[notCurrentlyPlaying()].setVolume(0);
    musicProx[currentlyPlayingMusic] = numPlayers;
    musicProx[currentlyPlayingMusic] = getClosestProx(numPlayers);
  }

  //get the one taht is playing the right prox song
  int keepPlaying = -1;
  for( unsigned int i = 0; i < musicProx.size(); i++){
    if (musicProx[i] == numPlayers)
      keepPlaying = i;
  }

  //switch the songs if needed
  if(keepPlaying == -1){
    sf::Time offset = music[notCurrentlyPlaying()].getPlayingOffset();
    playMusic( getTrack(trackNo, numPlayers), notCurrentlyPlaying() );
    music[notCurrentlyPlaying()].setPlayingOffset(offset);
    music[notCurrentlyPlaying()].setVolume(0);
    musicProx[notCurrentlyPlaying()] = numPlayers;
    currentlyPlayingMusic = notCurrentlyPlaying();
  } else {
    currentlyPlayingMusic = keepPlaying;
  }

  //update the volumes
  float volume = 0;
  for( unsigned int i = 0; i < music.size(); i++ ){
    if( i == currentlyPlayingMusic ){ //FADE IN
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
  if(!useSound) 
    return;

  if (music[index].openFromFile(musics[musicN])){
    music[index].play();
  }
}

void AudioManager::processPlayerSound(Player& o){
  static bool walk_toggle[4] = { false, false, false, false};

  if(o.charging) {
    playSound("c1", "cplayer:"+ o.player_id, o.getPosition());
  } else {
    stopSound( "cplayer:"+ o.player_id);
  }


  if(o.walking) {

    if(walk_toggle[o.player_id]) {
     
      walk_toggle[o.player_id] = false;
      playSound("w1", "w1player:"+o.player_id, o.getPosition());

    } else {     
      walk_toggle[o.player_id] = true;
      playSound("w2", "w2player:"+o.player_id, o.getPosition());
    }
   
  } else {
    stopSound("w1player:"+o.player_id);   
    stopSound("w2player:"+o.player_id);
  }
}

void AudioManager::processProjectileSound(Projectile& o){
  if(o.getFired()){
    playSound( "f1", "fire:"+o.id, o.getPosition());
  }
}

int AudioManager::notCurrentlyPlaying(){
  if(currentlyPlayingMusic == 0)
    return 1;
  return 0;
}

std::string AudioManager::getTrack( int track, int prox ){
  if( track == 0 ){
    switch(prox){
    case 0:
      return "m1_1";
    case 1:
      return "m1_2";
    case 2:
      return "m1_3";
    case 3:
      return "m1_4";
    }
  }
  else if( track == 1 ){
    switch(prox){
    case 0:
      return "m2_1";
    case 1:
      return "m2_2";
    case 2:
      return "m2_3";
    case 3:
      return "m2_4";
    }
  }
  std::cout << "error, trying to load track that does not exist" << std::endl;
  return "";
}

int AudioManager::getClosestProx( int prox ){
  ++prox;
  if(prox > 3)
    prox = 2;
  return prox;
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
