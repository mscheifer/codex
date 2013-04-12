#pragma once
#include <string>

class NetworkData{
public:
  virtual std::string toString(){
    return "NetworkData";
  }

  NetworkData(){};
  NetworkData(NetworkData& n){};
  ~NetworkData(){}
};

class NetworkData2 : public NetworkData{
public:
  std::string str1;
  std::string str2;

  std::string toString(){
    return "str 1: " + str1 + "\nstr2: " + str2 + "\n";
  }

  NetworkData2(){}
  NetworkData2(NetworkData2& n){
    str1 = n.str1;
    str2 = n.str2;
    //std::memcpy(this, &n, sizeof(n));
  }
  ~NetworkData2(){}


};