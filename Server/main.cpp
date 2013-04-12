
#include "Server.h"
#include "Client.h"
#include "Player.h"
#include <cctype> //this is for toupper method

int main(){
  Player allen (0,0,0);
  Player guru (1,1,1);
  std::cout<<"ALLEN ATTACK GURU"<<allen.attack(&guru)<<std::endl;
  std::cout<<typeid(allen).name()<<typeid(guru).name()<<std::endl;
  std::cout<<guru.getHealth()<<std::endl;
  std::string input;
  std::getline(std::cin, input);
  /*
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
  */
  return 0;
}