#ifndef GRAPHICS_CLIENT_H
#define GRAPHICS_CLEINT_H
#include <SFML/Window.hpp>
#include "displaySet.h"
#include "drawSet.h"
#include "light.h"
#include "userInput.h"

namespace gx {

class graphicsClient {
    sf::Window window;
    GLenum     glewStatus;

    //scene data
    //light light1;
    displaySet display;

    drawSet entities;

    sf::Clock fpsClock;
    int fpsFrames;

    void reshape(unsigned int, unsigned int);
    std::vector<gx::uniform::block*> uniforms();
  public:
    graphicsClient();
    graphicsClient(const graphicsClient&);// = delete;
    graphicsClient& operator=(const graphicsClient&);// = delete;
    graphicsClient(graphicsClient&&);// = delete;
    graphicsClient& operator=(graphicsClient&&);// = delete;
    userInput handleInput();
    void draw();

    void updateEntities(std::vector<std::pair<vector3,int>>);
};

} //end namespace gx
#endif //GRAPHICS_CLIENT_H