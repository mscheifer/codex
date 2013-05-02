#include "Server.h"
#include "Client.h"
#include "Player.h"
#include "boundingBox.h"
#include <cctype> //this is for toupper method

int main() {
  std::cout.sync_with_stdio(false); //speed up printing
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
