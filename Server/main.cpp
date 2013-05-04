#include "Server.h"
#include "Client.h"
#include "Player.h"
#include <cctype> //this is for toupper method

#include "boundingManager.h" //TODO remove this is for testing

int main(){  
  boxTest();
  sphereTest();
  std::cout << "Run (s)erver or (c)lient?: ";
  std::string input = ""; 
  std::getline(std::cin, input);
  if( std::toupper(input[0]) == 'S' ) {
    NetworkServer server;
    server.doServer();
  } else {
    NetworkClient client;
    client.doClient();
  }
  
  return 0;
}