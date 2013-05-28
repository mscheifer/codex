#ifndef GRAPHICS_CLIENT_H
#define GRAPHICS_CLEINT_H
#include <SFML/Window.hpp>
#include "displaySet.h"
#include "drawer.h"
#include "light.h"
#include "input.h"
#include "HUD.h"
#include "lobby.h"
#include "ClientGameTimeAction.h" 
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "scoreBoard.h"

class Entity;

namespace gx {
class graphicsClient {
    sf::RenderWindow window;
    GLenum     glewStatus;
    input      userInput;
    //scene data
    light light1;
    displaySet display;

    staticDrawer entities;
    dynamicDrawer animatedDrawer;

    //player info
    static const vector3f upDirection;

    vector3f playerDirection;
    vector3f playerStartDirection;
    vector3f playerStartRight;
    vector4f playerPosition;

    //packet info
    ClientGameTimeAction action;

    sf::Clock fpsClock;
    int fpsFrames;
    HUD Hud;
    lobby Lobby;
    scoreBoard Score;
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
    void updatePosition(vector4f);
    void updateEntities(std::vector<Entity*>);
    void updateHUD(Player & player);
    void drawLobby();
    void disableCursor();
    void enableCursor();
    bool gameStart() const;
    void gameEnd();
    void updateScores(std::vector<int> & pwins, std::vector<int> & pkills);

    //input functions
    bool closed()        { return this->userInput.getStop(); }
    bool jumped()        { return this->userInput.getJump(); }
    bool fire1 ()        { return this->userInput.fire1  (); }
    bool fire2 ()        { return this->userInput.fire2  (); }
    vector3f getDir()     { return this->playerDirection; }
};

} //end namespace gx
#endif //GRAPHICS_CLIENT_H
