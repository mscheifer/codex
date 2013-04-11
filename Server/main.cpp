
#include "Server.h"
#include "Client.h"
#include <cctype> //this is for toupper method

int main(){
  std::cout << "Run (s)erver or (c)lient?" << std::endl;
  std::string input = ""; 
  std::getline(std::cin, input);
  if( std::toupper(input[0]) == 'S' ){
    NetworkServer server;
    server.doServer();
  }
  else{
    NetworkClient client;
    client.doClient();
  }

  return 0;
}