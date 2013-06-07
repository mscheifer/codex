#include <cctype> //this is for toupper method
#include "Server.h"
#include "Client.h"
//#include <vld.h>

int main() {
  std::cout.sync_with_stdio(false); //speed up printing
  ConfigManager::readConfig();
  
  //gx::vector3f v1(0,1,0);
  //gx::vector3f v2(1,1,0);
  //gx::vector3f v3(-1,0,0);
  //std::cout << "angle " << atan2(v1.x*v2.y-v2.x*v1.y,v1.x*v2.x+v1.y*v2.y) * 180/M_PI ;
  //v2 = v3;
  //std::cout << "angle " << atan2(v1.x*v2.y-v2.x*v1.y,v1.x*v2.x+v1.y*v2.y) * 180/M_PI ; 
  //v2 = gx::vector3f(0,1,0);
  //std::cout << "angle " << atan2(v1.x*v2.y-v2.x*v1.y,v1.x*v2.x+v1.y*v2.y) * 180/M_PI ; 
  //
  //v1 = gx::vector3f(1,1,0);
  //v2 = gx::vector3f(0,-1,0);
  //std::cout << "angle " << atan2(v1.x*v2.y-v2.x*v1.y,v1.x*v2.x+v1.y*v2.y) * 180/M_PI ; 

  std::cout << "Run (s)erver or (c)lient?: ";
  std::string input = ""; 
  std::getline(std::cin, input);
  if( std::toupper(input[0]) == 'S' ) {
    ConfigManager::setupLog("server");
    NetworkServer server;
    server.doServer();
  } else {
    ConfigManager::setupLog("client");
    NetworkClient client;
    client.doClient();
  }
  while(true) {} //keep command line window open
  
  return 0;
}
