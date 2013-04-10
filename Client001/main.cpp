#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace sf;

////////////////////////////CLIENT//////////////////////////////
////////////////////////////////////////////////////////////////
int main()
{
    IpAddress myIpAddress = IpAddress::getLocalAddress();
    cout << "Client Ip Address: " << myIpAddress.toString() << endl;

    //input is ipaddress to connect to
    string input = myIpAddress.toString(); //"192.168.1.71";
    sf::TcpSocket socket;
    bool validIpAddress = true;
    bool pressedLeft = false;
    bool pressedRight = false;
    Socket::Status s;

    do{
      getline(cin, input);

      // Create a socket and connect it to 192.168.1.71 on port 55001
      s = socket.connect(input, 55001);

      if(s != Socket::Status::Error)
        validIpAddress = false;
      else
        cout << "try again, " << input << " is an invalid ip address" << endl;

    } while(validIpAddress);

    socket.setBlocking(false);
    
    while(true){
      std::string message;

      // Send a message to the connected host{
      if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !pressedLeft){
        message = "pressed left";
        cout << "sending: " << message << endl;
        socket.send(message.c_str(), message.size() + 1);
        pressedLeft = true;
      } else if( !sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        pressedLeft = false;
      }

      if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !pressedRight){
        message = "pressed right";
        cout << "sending: " << message << endl;
        socket.send(message.c_str(), message.size() + 1);
        pressedRight = true;
      } else if( !sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        pressedRight = false;
      }

      // Receive an answer from the server
      char buffer[1024];
      std::size_t received = 0;

      TcpSocket::Status stat = socket.receive(buffer, sizeof(buffer), received);
      if( stat == Socket::Done ){
        std::cout << "The server said: " << buffer << std::endl;
        //cout << "status " << stat << endl;
      }
    }

    getline(cin, input);
    cout << input << endl;
    getline(cin, input);
}