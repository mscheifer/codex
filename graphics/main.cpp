#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include "displaySet.h"
#include "shaderProgram.h"
#include "vao.h"

void reshape(gx::displaySet& display, unsigned int w, unsigned int h) {
  typedef gx::displaySet::elem_t elem_t;
  const elem_t fov       = 53.13f;
  const elem_t ratio     = elem_t(w) / elem_t(h);
  const elem_t nearPlane = 1.0f;
  const elem_t farPlane  = 30.0f;
  // adjust the viewport when the window is resized
  glViewport(0, 0, w, h);
  display.setProjection(fov,ratio,nearPlane,farPlane);
}

int main() {
  const unsigned int defaultWindowWidth  = 800;
  const unsigned int defaultWindowHeight = 600;

  // set openGL 3 context
/*
  //keep this uncommented for now because of weird bug
  sf::ContextSettings oglContext;
  oglContext.depthBits = 24;
  oglContext.stencilBits = 8;
  oglContext.antialiasingLevel = 0;
  oglContext.majorVersion = 3;
  oglContext.minorVersion = 1;
*/
  // create the window
  sf::Window window(sf::VideoMode(defaultWindowWidth, defaultWindowHeight),
    "DrChao", sf::Style::Default);//, oglContext);
  window.setVerticalSyncEnabled(false);

  // load resources, initialize the OpenGL states, ...
  GLenum glewErr = glewInit();
  if(GLEW_OK != glewErr) {
    std::cout << "error initializing GLEW" << std::endl;
    exit(1);
  }

  //stores the next available uniform bind point
  //should be incremented every time it is used
  GLuint newUniformBindPoint = 0;

  gx::displaySet display(newUniformBindPoint++);

  display.setView({10.0, 2.0, 10.0},{0.0, -2.0, -5.0},{0.0, 1.0, 0.0});
  reshape(display,defaultWindowWidth, defaultWindowHeight);


  const std::vector<std::pair<const std::string,GLuint>> uniforms = 
    { std::make_pair("display", display.bindPoint()) };

  const GLuint posAttribLoc = 0;
  const GLuint colorAttribLoc = 1;

  const std::vector<std::pair<const std::string,GLuint>> attribs = 
    { std::make_pair("position", posAttribLoc),
      std::make_pair("color"   , colorAttribLoc) };

  gx::shaderProgram prog =
    gx::shaderFromFiles("default.vert","default.frag",uniforms,attribs);

  glEnable(GL_DEPTH_TEST); //enable depth buffer
  glClearColor(1.0,1.0,1.0,1.0); //start with a white screen
  glClearDepth(1.f);
  glDepthFunc(GL_LEQUAL);

  std::vector<GLfloat> positions = { 0.0f, 1.0f,-5.0f, 1.0f,
                                     1.0f, 0.0f,-5.0f, 1.0f,
                                    -1.0f,-1.0f,-5.0f, 1.0f};
  std::vector<GLfloat> colors    = { 0.0f, 0.0f, 1.0f, 1.0f,
                                     0.0f, 0.0f, 1.0f, 1.0f,
                                     0.0f, 0.0f, 1.0f, 1.0f};
  std::vector<GLuint>  indices   = { 0, 1, 2 };

  gx::vertexAttrib positionsAttrib(posAttribLoc  ,4,0,positions);
  gx::vertexAttrib colorsAttrib   (colorAttribLoc,4,0,colors);

  gx::vao testTri(indices,{ positionsAttrib, colorsAttrib });

  //fps setup
  sf::Clock fpsClock;
  float fpsFrames = 0;

  // run the main loop
  bool running = true;
  while (running)
  {
    // handle events
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
      {
        // end the program
        running = false;
      }
      else if (event.type == sf::Event::Resized)
      {
        reshape(display,event.size.width, event.size.height);
      }
    }

    // clear the buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // draw...
    prog.use();
    testTri.draw();

    // end the current frame (internally swaps the front and back buffers)
    window.display();

    //fps calc
    fpsFrames++;
    if(fpsClock.getElapsedTime().asSeconds() >= 1) {
      std::cout << "fps: " << fpsFrames << std::endl;
      fpsFrames = 0;
      fpsClock.restart();
    }

    GLenum err;
    while(gx::debugOn && (err = glGetError())) {
      std::cout << "OpenGL error: " << err << std::endl;
    }
  }

  // release resources...

  return 0;
}
