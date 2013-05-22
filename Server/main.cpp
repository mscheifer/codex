#include <cctype> //this is for toupper method
#include "Server.h"
#include "Client.h"

int main() {

  std::cout.sync_with_stdio(false); //speed up printing
  ConfigManager::readConfig();

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
