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
  gx::debugout << "glViewport(0, 0, w, h);" << gx::endl;
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
  //make sure this is above all other calls so that the desctructor is called
  //last so we have an opengl context for destructors
  sf::Window window(sf::VideoMode(defaultWindowWidth, defaultWindowHeight),
    "DrChao", sf::Style::Default);//, oglContext);
  window.setVerticalSyncEnabled(false);

  // load resources, initialize the OpenGL states, ...
  GLenum glewErr = glewInit();
  if(GLEW_OK != glewErr) {
    std::cout << "error initializing GLEW" << std::endl;
    exit(1);
  }

  gx::displaySet display;

  display.setView(gx::vector3( 5.0,  2.0,  5.0),
                  gx::vector3(-4.0, -2.0, -5.0),
                  gx::vector3( 0.0,  1.0,  0.0));
  reshape(display,defaultWindowWidth, defaultWindowHeight);

  glEnable(GL_DEPTH_TEST); //enable depth buffer
  glClearColor(1.0,1.0,1.0,1.0); //start with a white screen
  glClearDepth(1.f);
  glDepthFunc(GL_LEQUAL);

  std::array<GLfloat,8*4> posArray    = {{ 0.0f, 0.0f, 0.0f, 1.0f,
                                           1.0f, 0.0f, 0.0f, 1.0f,
                                           0.0f, 1.0f, 0.0f, 1.0f,
                                           1.0f, 1.0f, 0.0f, 1.0f,
                                           0.0f, 0.0f, 1.0f, 1.0f,
                                           1.0f, 0.0f, 1.0f, 1.0f,
                                           0.0f, 1.0f, 1.0f, 1.0f,
                                           1.0f, 1.0f, 1.0f, 1.0f }};
  std::array<GLfloat,8*4> colorsArray = {{ 0.0f, 0.0f, 1.0f, 1.0f,
                                           0.0f, 0.0f, 1.0f, 1.0f,
                                           0.0f, 0.0f, 1.0f, 1.0f,
                                           0.0f, 0.0f, 1.0f, 1.0f,
                                           0.0f, 0.0f, 1.0f, 1.0f,
                                           0.0f, 0.0f, 1.0f, 1.0f,
                                           0.0f, 0.0f, 1.0f, 1.0f,
                                           1.0f, 0.0f, 1.0f, 1.0f }};
  std::array<GLuint,6*6>  indicesArray = {{ 0, 1, 2, 1, 3, 2,
                                            2, 3, 6, 3, 7, 6,
                                            4, 0, 6, 0, 2, 4,
                                            1, 5, 3, 5, 7, 3,
                                            4, 5, 0, 5, 1, 0,
                                            5, 4, 7, 4, 6, 7 }};

  std::vector<GLfloat> positions(posArray.begin(),   posArray.end());
  std::vector<GLfloat> colors(colorsArray.begin(),   colorsArray.end());
  std::vector<GLuint>  indices(indicesArray.begin(), indicesArray.end());

  gx::vertexAttrib positionsAttrib("position",4,0,positions);
  gx::vertexAttrib colorsAttrib   ("color"   ,4,0,colors);

  std::vector<const gx::vertexAttrib*> attribs;
  attribs.push_back(&positionsAttrib);
  attribs.push_back(&colorsAttrib);

  gx::vao testTri(indices,attribs);


  /*const std::vector<std::pair<const std::string,GLuint>> uniforms = 
    { std::make_pair("display", display.bindPoint()) }; */

  //yet another visual c++ hack
  std::vector<std::pair<const std::string,GLuint>> uniforms;
  uniforms.push_back(std::make_pair("display", display.bindPoint()));

  gx::shaderProgram prog =
    gx::shaderFromFiles("default.vert","default.frag",uniforms,attribs);

  //fps setup
  sf::Clock fpsClock;
  float fpsFrames = 0;

  // run the main loop
  bool running = true;
  while (running) {
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
    gx::debugout << "glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT";
    gx::debugout << "| GL_STENCIL_BUFFER_BIT);" << gx::endl;

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
  }

  return 0;
}
