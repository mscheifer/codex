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

  display.setView(gx::vector3(10.0,  2.0, 10.0),
	              gx::vector3( 0.0, -2.0, -5.0),
				  gx::vector3( 0.0,  1.0,  0.0));
  reshape(display,defaultWindowWidth, defaultWindowHeight);

  /*const std::vector<std::pair<const std::string,GLuint>> uniforms = 
    { std::make_pair("display", display.bindPoint()) }; */

  //yet another visual c++ hack
  std::vector<std::pair<const std::string,GLuint>> uniforms;
  uniforms.push_back(std::make_pair("display", display.bindPoint()));

  const GLuint posAttribLoc = 0;
  const GLuint colorAttribLoc = 1;

  /*const std::vector<std::pair<const std::string,GLuint>> attribs = 
    { std::make_pair("position", posAttribLoc),
      std::make_pair("color"   , colorAttribLoc) };*/
  std::vector<std::pair<const std::string,GLuint>> attrib_info;
  attrib_info.push_back(std::make_pair("position", posAttribLoc));
  attrib_info.push_back(std::make_pair("color"   , colorAttribLoc));
  
  gx::shaderProgram prog =
    gx::shaderFromFiles("default.vert","default.frag",uniforms,attrib_info);

  prog.use();

  glEnable(GL_DEPTH_TEST); //enable depth buffer
  glClearColor(1.0,1.0,1.0,1.0); //start with a white screen
  glClearDepth(1.f);
  glDepthFunc(GL_LEQUAL);

  std::array<GLfloat,12> posArray    = { 0.0f, 1.0f,-5.0f, 1.0f,
                                         1.0f, 0.0f,-5.0f, 1.0f,
                                        -1.0f,-1.0f,-5.0f, 1.0f};
  std::array<GLfloat,12> colorsArray = { 0.0f, 0.0f, 1.0f, 1.0f,
                                         0.0f, 0.0f, 1.0f, 1.0f,
                                         0.0f, 0.0f, 1.0f, 1.0f};
  std::array<GLuint,3>  indicesArray = { 0, 1, 2 };

  std::vector<GLfloat> positions(posArray.begin(),   posArray.end());
  std::vector<GLfloat> colors(colorsArray.begin(),   colorsArray.end());
  std::vector<GLuint>  indices(indicesArray.begin(), indicesArray.end());

  gx::vertexAttrib positionsAttrib(posAttribLoc  ,4,0,positions);
  gx::vertexAttrib colorsAttrib   (colorAttribLoc,4,0,colors);

  std::vector<gx::vertexAttrib> attribs;
  attribs.push_back(positionsAttrib);
  attribs.push_back(colorsAttrib);

  gx::vao testTri(indices,attribs);

  //fps setup
  sf::Clock fpsClock;
  float fpsFrames = 0;

  // run the main loop
  bool running = true;
  for(int asdf = 0; asdf < 3; ++asdf)
  //while (running)
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
    gx::debugout << "glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);" << gx::endl;

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
  while(true) {}

  // release resources...

  return 0;
}
