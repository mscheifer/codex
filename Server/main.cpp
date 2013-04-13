
#include "Server.h"
#include "Client.h"
#include "Player.h"
#include <cctype> //this is for toupper method

int main(){
 /* Player Allen(1,2,3);
  Allen.moveForward();
  std::cout<<Allen.getString()<<std::endl;
  std::string bleh;
  std::cin>>bleh;*/
  
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