#pragma once
#include <string>
#include <SFML/Network.hpp>

class sampleObject {
  std::vector<std::string> a;
  int b;
  float c;
  sampleObject() {
    
  }
  void init(){
    a.push_back("hello");
    a.push_back("this is a test");
    b = -5;
    c=3.14;
  }
  void seraialize(sf::Packet & packet ) {
    packet<<a.size();
    for (int i=0;i<a.size();i++) 
      packet <<a[i];
    packet <<b;
    packet <<c;
  }
  void desrialize(sf::Packet & packet ) {
    int size;
    packet >> size;
    std::string temp;
    for (int i=0;i<size; i++) {
      packet >> temp;
      a.push_back(temp);
    }
    packet >> b;
    packet >> c;
  }
};

