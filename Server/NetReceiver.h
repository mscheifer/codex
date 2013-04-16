#pragma once

#include <stdint.h>
#include <SFML/Network.hpp>
#include "NetworkPacket.h"
#include <algorithm>

  const int maxSize = 9000;
  const int sizeSize = 4;

class NetReceiver{
public:
  //private:
  unsigned char networkBuffer[maxSize];
  unsigned char networkSizeBuffer[sizeSize];
  int state; //waiting, recv size, recv data
  uint32_t netLength;
  uint32_t netReceived;
  uint32_t netRead;
  sf::TcpSocket * socket;

public:
  NetReceiver(sf::TcpSocket * socket){
    netRead = 0;
    netReceived = 0;
    state = 0;
    this->socket = socket;
  }

  void sendMessage(NetworkPacket & netPack){
    uint32_t size = sizeof(netPack);
    unsigned char sizeBuffer[sizeSize+1];
    sizeBuffer[0] = (unsigned char)(size >> 24);
    sizeBuffer[1] = (unsigned char)(size >> 16);
    sizeBuffer[2] = (unsigned char)(size >> 8);
    sizeBuffer[3] = (unsigned char) size;

    socket->send(sizeBuffer, sizeof(uint32_t));
    socket->send(&netPack, sizeof(netPack));
  }

  bool receiveMessage(){
    std::size_t received = 0;
    sf::TcpSocket::Status stat;
    
    //check if anything has been recieved
    if( state == 0 ){
       stat = socket->receive(networkSizeBuffer+netRead, 1, received);
    } else{
       stat = socket->receive(networkBuffer+netRead, 1, received);
    }

    if( stat == sf::Socket::Done ){
      netRead++; //because I already read 1 byte

      if(state == 0){ //reading in size
        socket->receive(networkSizeBuffer+netRead, sizeSize-netRead, received);
        netRead += std::min( sizeSize-netRead, received );
        
        if( netRead == sizeSize ){
          netLength = 0;
          netLength |= (networkSizeBuffer[0] << 24);
          netLength |= (networkSizeBuffer[1] << 16);
          netLength |= (networkSizeBuffer[2] << 8);
          netLength |= (networkSizeBuffer[3]);

          netRead = 0;
          state = 1;
        }
      }

      if (state == 1){ //receiving data
        socket->receive(networkBuffer+netRead, netLength-netRead, received);
        netRead += std::min( netLength-netRead, received );
        
        if( netRead == netLength ){
          netRead = 0;
          state = 2;
        }
      }

      if (state == 2){ //done
        state = 0;
        netRead = 0;
        return true;
      }

    }
    return false;
  }

};