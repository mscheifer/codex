#include <GL/glew.h>
#include "StaticEnums.h"

namespace sf {
class Window;
class Event;
};

namespace gx {
template<typename T>
class vector3;

typedef vector3<GLfloat> vector3f;

class input {
    bool   updated;
    bool   jumped;
    bool   stopped;
    bool   fired1;
    bool   fired2;
    bool   pickup;
    bool   switchWeapon;
    unsigned int width;
    unsigned int height;
    bool resized;
    int  mouseX;
    int  mouseY;
  public:
    input();
    bool    getUpdated();
    bool       getJump();
    bool       getStop();
    bool         fire1() const;
    bool         fire2() const;
    bool        pickUp();
    bool       switchW();
    bool         drawS();
    move_t  movePlayer();
    vector3f turnPlayer();

    bool resizedWindow();
    unsigned int windowWidth();
    unsigned int windowHeight();

    int mouseXpos() const;
    int mouseYpos() const;

    void setUpMouse();
    void handle(sf::Window&);
    void handleEvent(const sf::Event&);
};
} //end namespace gx
