#ifndef GRAPHICS_CLIENT_H
#define GRAPHICS_CLEINT_H
#include <SFML/Window.hpp>
#include "displaySet.h"
#include "drawer.h"
#include "SkyboxDraw.h"
#include "light.h"
#include "input.h"
#include "HUD.h"
#include "lobby.h"
#include "ClientGameTimeAction.h" 
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "scoreBoard.h"
#include "StaticEntity.h"

class Entity;

namespace gx {
class graphicsClient {
    sf::RenderWindow window;
    GLenum     glewStatus;
    input      userInput;
    //scene data
    light lights;
    displaySet display;
    staticDrawer entities;
    dynamicDrawer animatedDrawer;
    SkyboxDraw skyboxDrawer;
    particleDrawer particles;

    HUD Hud;
    lobby Lobby;
    scoreBoard Score;

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
    void addStaticInstance();
    void updatePosition(vector4f);
    void updateDirection(vector3f);
    void setStaticEntities(std::vector<StaticEntity*> e);
    void clearEntities();
    void addEntity(Entity*);
    void addEntity(Player*);
    void addEntity(Projectile*);
    void updateHUD(Player & player);
    void drawLobby();
    void disableCursor();
    void enableCursor();
    bool gameStart() const;
    void gameEnd();
    void updateScores(std::vector<int> & pwins, std::vector<int> & pkills, std::vector<bool> & pdead);
    void setMinotaur(unsigned int playerid);
    void updateLobby(std::vector<std::pair<int,bool>> & playerStatus );
    void updateHUDTimer(float timer); 
    std::string getInputText() const;
    void setConnected(bool connected);
    void updateNames(std::vector<std::string> & names);
    bool closed()        { return this->userInput.getStop(); }
    void setPlayerId(unsigned int x) { Score.setPlayerId(x); }
};

} //end namespace gx
#endif //GRAPHICS_CLIENT_H
