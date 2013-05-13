#include <cctype> //this is for toupper method
#include "Server.h"
#include "Client.h"
#include "Player.h"

int main() {
  std::cout.sync_with_stdio(false); //speed up printing


  ConfigManager::readConfig();

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
