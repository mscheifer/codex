#include "AudioManager.h"
#include "ConfigManager.h"
#include "util.h"

std::array<sf::Music,6> AudioManager::music;
std::array<proxStruct,2> AudioManager::musicProx;
std::map<std::string, sf::SoundBuffer*> AudioManager::soundBuffers;
std::map<std::string, std::string> AudioManager::musics;
std::map<std::string, sf::Sound> AudioManager::sounds;
std::map<std::string, sf::Sound> AudioManager::playerSounds[4];
bool AudioManager::useSound;
int AudioManager::trackNo;
float AudioManager::soundScaling;
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
  
  soundScaling = StringToNumber<float>(ConfigManager::configMap["soundScaling"]);
  trackNo = 0;
  currentlyPlayingMusic = 0;
  for(unsigned int i = 0; i < musicProx.size(); i++){
    musicProx[i] = proxStruct(-1,false);
  }

  static int i = 0;
  loadSound("sc1", "sounds/scream_1.wav");
  
  loadSound("c1", "sounds/charge1.wav");
  
  loadSound("w1", "sounds/foot_step_1.wav");
  loadSound("w2", "sounds/foot_step_2.wav");

  //loadSound("f1", "sounds/fire_1.wav");
  loadSound("melee", "sounds/52458__audione__sword-01_.wav");
  loadSound("combine", "sounds/combine.wav");

  loadSound("weapIce", "sounds/weaponIce.wav");
  loadSound("weapBasic", "sounds/weaponBasic.wav");
  loadSound("weapFire", "sounds/weaponFire.wav");
  loadSound("weapThu", "sounds/weaponThu.wav");
  loadSound("weapFist", "sounds/weaponFist.wav");

  loadSound("shootFir", "sounds/shootFir.wav");
  loadSound("shootIce", "sounds/shootIce.wav");
  loadSound("shootThu", "sounds/shootThu.wav");
  loadSound("shootBasic", "sounds/shootBasic.wav");
  loadSound("shootGrav", "sounds/shootGrav.wav");
        
  loadSound("collectPowerup", "sounds/collectPowerup.wav");

  //track 0
  musics["m1_0_t"] = "sounds/m1_0_t.wav";
  musics["m1_1_t"] = "sounds/m1_1_t.wav";
  musics["m1_2_t"] = "sounds/m1_2_t.wav";
  musics["m1_0_f"] = "sounds/m1_0_f.wav";
  musics["m1_1_f"] = "sounds/m1_1_f.wav";
  musics["m1_2_f"] = "sounds/m1_2_f.wav";

  ////track 1
  //musics["m2_1"] = "sounds/m2_1.wav";
  //musics["m2_2"] = "sounds/m2_2.wav";
  //musics["m2_3"] = "sounds/m2_3.wav";
  //musics["m2_4"] = "sounds/m2_4.aif";
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

void AudioManager::stopPlayerSound(int player_id, std::string key) {
   if( playerSounds[player_id].find(key) != playerSounds[player_id].end()) {
     playerSounds[player_id].find(key)->second.stop();
     playerSounds[player_id].erase(key);
   }
}
void AudioManager::playSoundHelper( sf::Sound* s, v3_t pos, sf::SoundBuffer* sbuff){
  s->setPosition(pos.x/soundScaling,pos.y/soundScaling,pos.z/soundScaling);
  if(s->getStatus() != sf::Sound::Playing)  {
    s->setBuffer(*sbuff);
    s->play();
  }

}

void AudioManager::updateMusic( int numPlayers, bool minotaur ){
  if(!useSound)
    return;

  static bool useMusic = StringToNumber<int>(ConfigManager::configMap["music"]) == 1;
  if(!useMusic)
    return;

  //less intensive way
  //static int index = -1;

  //if(music[0].getStatus() != sf::Sound::Playing){
  //  loadTrack(trackNo);
  //  index =  minotaur ? numPlayers : numPlayers + 3;
  //  music[index].setVolume(100);
  //}

  //int temp = minotaur ? numPlayers : numPlayers + 3;
  //if(temp != index){
  //  index = temp;
  //  
  //  for( unsigned int i = 0; i < music.size(); i++ ){
  //    if( i == index ){
  //      music[i].setVolume(100);
  //    } else {
  //      music[i].setVolume(0);
  //    }
  //  }
  //
  //}

  ////switch tracks too cpu intensive way of doing it
  if(music[0].getStatus() != sf::Sound::Playing){
    //++trackNo; only 1 track
    //trackNo = trackNo % maxTracks;
    playMusic( getTrack(trackNo, numPlayers, minotaur), currentlyPlayingMusic );
    playMusic( getTrack(trackNo, getClosestProx(numPlayers), minotaur), notCurrentlyPlaying() );

    //set the track to currently play
    music[currentlyPlayingMusic].setVolume(100);
    music[notCurrentlyPlaying()].setVolume(0);
    musicProx[currentlyPlayingMusic] = proxStruct(numPlayers,minotaur);
    musicProx[currentlyPlayingMusic] = proxStruct(getClosestProx(numPlayers),minotaur);
  }

  //get the one taht is playing the right prox song
  int keepPlaying = -1;
  for( unsigned int i = 0; i < musicProx.size(); i++){
    if (musicProx[i].prox == numPlayers && musicProx[i].minotaur == minotaur)
      keepPlaying = i;
  }

  //switch the songs if needed
  if(keepPlaying == -1){
    music[notCurrentlyPlaying()].setVolume(0);
    playMusic( getTrack(trackNo, numPlayers, minotaur), notCurrentlyPlaying() );
    music[notCurrentlyPlaying()].setPlayingOffset(music[currentlyPlayingMusic].getPlayingOffset());
    musicProx[notCurrentlyPlaying()] = proxStruct(numPlayers,minotaur);
    currentlyPlayingMusic = notCurrentlyPlaying();
  } else {
    currentlyPlayingMusic = keepPlaying;
  }

  //update the volumes
  float volume = 0;
  for( unsigned int i = 0; i < music.size(); i++ ){
    if( i == currentlyPlayingMusic ){ //FADE IN
      volume = music[i].getVolume();
      volume += 3;
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
    playMusic("m1_0_f", 0);
    playMusic("m1_1_f", 1);
    playMusic("m1_2_f", 2);
    playMusic("m1_0_t", 3);
    playMusic("m1_1_t", 4);
    playMusic("m1_2_t", 5);
  } /*else if( i == 1 ){
    playMusic("m2_1", 0);
    playMusic("m2_2", 1);
    playMusic("m2_3", 2);
    playMusic("m2_4", 3);
  }*/

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

void AudioManager::playPlayerSound(std::string sound, int player_id , std::string name, v3_t pos) {
  int copy_id = player_id;
  std::map<std::string,sf::SoundBuffer*>::iterator it = soundBuffers.find(sound);

  if(it != soundBuffers.end()){ //element exists

    if(playerSounds[copy_id].find(name) ==  playerSounds[copy_id].end()) {
      playerSounds[copy_id].insert(std::pair<std::string,sf::Sound>(name,sf::Sound()));
    }

    playSoundHelper( & playerSounds[copy_id].find(name)->second ,pos, it->second);
  
  }
}

void AudioManager::updatePlayerSoundsPosition(int player_id , v3_t pos ) {
 std::map<std::string,sf::Sound>::iterator it =  playerSounds[player_id].begin();
 while( it != playerSounds[player_id].end()) {
   it->second.setPosition(pos.x/soundScaling,pos.y/soundScaling,pos.z/soundScaling);
   it++;
 }

}
void AudioManager::processPlayerSound(Player& o){
  static bool walk_toggle[4] = { false, false, false, false};
  updatePlayerSoundsPosition(o.player_id , o.getPosition());

  if(o.meleeAttack)
    playSound("melee", "melee", o.getPosition());

  if(o.weaponCall){
    std::stringstream ss; 
    ss << "playerCall: " << o.player_id;
    stopPlayerSound( o.player_id, ss.str() );
    playPlayerSound(getWeaponCall(o.weaponCallType), o.player_id, ss.str(), o.getPosition());
  }

  if(o.collectPowerUp){
    std::stringstream ss;
    ss << "playcerCollectPowerUp: " << o.player_id;
    playSound("collectPowerup", ss.str(), o.getPosition());
  }

  if(o.charging) {
//    std::cout << " player is charging"<< std::endl;
    playPlayerSound("c1", o.player_id,  "charging", o.getPosition());
  } else {
     stopPlayerSound( o.player_id , "charging");
  }

  if(o.shotProjectile) {  
    std::stringstream ss;

    ss << id << "fplayer:" << o.player_id;
    playSound(getShootSound(o.chargeMagicType), ss.str(), o.getPosition());
    id++;
  }

  if(o.attacked) {  
    std::stringstream ss;
  
    static int id = 0;

    ss << id << "scplayer:" << o.player_id;
    playSound("sc1", ss.str(), o.getPosition());
    id++;
  }


  if(o.walking) {
    if(walk_toggle[o.player_id]) {
     
      walk_toggle[o.player_id] = false;
      std::stringstream ss;
      ss << "w1player:" << o.player_id;
      playSound("w1",ss.str(), o.getPosition());

    } else { 
      walk_toggle[o.player_id] = true;
      std::stringstream ss;
      ss <<"w2player:" << o.player_id;
      playSound("w2", ss.str(), o.getPosition());
    }
   
  } else {
    std::stringstream ss;
    ss << "w1player:" << o.player_id;
    stopSound(ss.str());   
    std::stringstream ss2;
    ss2 <<"w2player:" << o.player_id;
    stopSound(ss2.str());
  }
}

void AudioManager::processProjectileSound(Projectile& o){
  //if(o.getFired()){
  //  playSound( "f1", "fire:"+o.id, o.getPosition());
  //} else 
  if (o.combined){
    playSound( "combine", "combine:"+o.id, o.getPosition());
  }
}

int AudioManager::notCurrentlyPlaying(){
  if(currentlyPlayingMusic == 0)
    return 1;
  return 0;
}

std::string AudioManager::getWeaponCall(WeaponType w){
  switch(w){
  case FIRE:
    return "weapFire";
  case ICE:
    return "weapIce";
  case THUNDER:
    return "weapThu";
  case BASIC:
    return "weapBasic";
  default:
    return "weapFist";
  }
}

std::string AudioManager::getShootSound(MAGIC_POWER m){
  switch(m){
  case FIR1:
  case FIR2:
  case FIR3:
    return "shootFir";
  case ICE1:
  case ICE2:
  case ICE3:
    return "shootIce";
  case THU1:
  case THU2:
  case THU3:
    return "shootThu";
  case B1:
    return "shootBasic";
  default:
    return "shootGrav";
  }
}

std::string AudioManager::getTrack( int track, int prox, bool minotaur ){
  if( track == 0 ){
    switch(prox){
    case 0:
      if(minotaur)
        return "m1_0_t";
      else
        return "m1_0_f";
    case 1:
      if(minotaur)
        return "m1_1_t";
      else
        return "m1_1_f";
    case 2:
      if(minotaur)
        return "m1_2_t";
      else
        return "m1_2_f";
    }
  }
  std::cout << "error, trying to load track that does not exist" << std::endl;
  return "";
}

int AudioManager::getClosestProx( int prox ){
  ++prox;
  if(prox > 2)
    prox = 0;
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
