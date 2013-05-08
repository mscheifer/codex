#include "StaticEnums.h"

namespace sf {
class Window;
class Event;
};

namespace gx {
class vector3;

class input {
    bool   updated;
    bool   jumped;
    bool   stopped;
    bool   fired1;
    bool   fired2;
    unsigned int width;
    unsigned int height;
    bool resized;
  public:
    input();
    bool    getUpdated();
    bool       getJump();
    bool       getStop();
    bool         fire1();
    bool         fire2();
    move_t  movePlayer();
    vector3 turnPlayer();

    bool resizedWindow();
    unsigned int windowWidth();
    unsigned int windowHeight();

    void setUpMouse();
    void handle(sf::Window&);
    void handleEvent(const sf::Event&);
};
} //end namespace gx
