#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace sf;

//////////////////////////////////SERVER//////////////////////////////////
//////////////////////////////////////////////////////////////////////////
int main()
{
  IpAddress myIpAddress = IpAddress::getLocalAddress();
  std::cout << "Server Ip Address: " << myIpAddress.toString() << endl;

  sf::TcpListener listener;
  listener.setBlocking(false);
  listener.listen(55001);
  vector<TcpSocket*> clients(10,0);
  int n = 0;
  clients[n] = new TcpSocket;

  while(true){

    // Wait for a connection
    if ( listener.accept(*clients[n]) == sf::Socket::Done ){
      std::cout << "New client connected: " << clients[n]->getRemoteAddress() << std::endl;
      clients[n]->setBlocking(false);
      n++;
      clients[n] = new TcpSocket;
    }

    // Receive a message from the client
    char buffer[1024];
    std::size_t received = 0;

    for( int i = 0; i < n; i++){

      TcpSocket::Status stat = clients[i]->receive(buffer, sizeof(buffer), received);
      if( stat == Socket::Done ){
        std::string message = clients[i]->getRemoteAddress().toString() + ": " + buffer;
        std::cout << message << endl;

        //Send an answer
        for( int j = 0; j < n; j++){
            clients[j]->send(message.c_str(), message.size() + 1);
        }

      }
    }
  }

  string input = "";
  std::getline(cin, input);
  std::cout << input << endl;
  std::getline(cin, input);
}