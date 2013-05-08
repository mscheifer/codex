#ifndef GRAPHICS_CLIENT_H
#define GRAPHICS_CLEINT_H
#include <SFML/Window.hpp>
#include "displaySet.h"
#include "drawSet.h"
#include "light.h"
#include "input.h"
#include "ClientGameTimeAction.h" 

namespace gx {

class graphicsClient {
    sf::Window window;
    GLenum     glewStatus;
    input      userInput;
    //scene data
    light light1;
    displaySet display;

    drawSet entities;

    //player info
    static const vector3 upDirection;

    vector3 playerDirection;
    vector3 playerStartDirection;
    vector3 playerStartRight;
    vector4 playerPosition;

    //packet info
    ClientGameTimeAction action;

    sf::Clock fpsClock;
    int fpsFrames;

    void setCamera();
    void reshape(unsigned int, unsigned int);
    std::vector<gx::uniform::block*> uniforms();
  public:
    graphicsClient();
    graphicsClient(const graphicsClient&);// = delete;
    graphicsClient& operator=(const graphicsClient&);// = delete;
    graphicsClient(graphicsClient&&);// = delete;
    graphicsClient& operator=(graphicsClient&&);// = delete;
    ClientGameTimeAction handleInput();
    void draw();
    void updatePosition(vector4);
    void updateEntities(std::vector<std::pair<vector4,int>>);

    //input functions
    bool closed()        { return this->userInput.getStop(); }
    bool jumped()        { return this->userInput.getJump(); }
    bool fire1 ()        { return this->userInput.fire1  (); }
    bool fire2 ()        { return this->userInput.fire2  (); }
    move_t getMovement() { return this->userInput.movePlayer(); }
    vector3 getDir()     { return this->playerDirection; }
};

} //end namespace gx
#endif //GRAPHICS_CLIENT_H
