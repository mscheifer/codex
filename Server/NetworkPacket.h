#pragma once
#include <SFML/Network.hpp>
#include "NetworkData.h"

class NetworkPacket{

private:

public:
  static const int maxPacketSize = 1024*4;
  enum Opcode {T1, T2};

  struct Type1{
    char s1[100];
    char s2[100];
  };

  struct Type2{
    int a;
    char s2[100];
  };

  Opcode opcode;

  union {
    Type1 t1;
    Type2 t2;
  };

  NetworkPacket(){};
  ~NetworkPacket(){};

  std::string getType2Data(){
    return "s1:" + std::string(t1.s1) + "\ns2: " + std::string(t1.s2);
  }
};