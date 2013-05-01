#include "graphicsClient.h"
#include <fstream>
#include "oglUtil.h"

namespace {
const unsigned int defaultWindowWidth  = 800;
const unsigned int defaultWindowHeight = 600;

std::string readFile(const std::string fileName) {
  std::ifstream vsFile(fileName);
  std::string fullSource = "";

  if(vsFile.is_open()) {
    std::string line;
    while(vsFile.good()) {
      getline(vsFile,line);
      fullSource += line + "\n";
    }
    vsFile.close();
  } else {
    std::cout << "error: cannot open file: " << fileName << std::endl;
  }
  return fullSource;
}

std::vector<gx::drawSet::vaoData_t> entitiesData() {
    //setup drawing data
  std::array<GLfloat,8*4> posArray    = {{ 0.0f, 0.0f, 0.0f, 1.0f,
                                           1.0f, 0.0f, 0.0f, 1.0f,
                                           0.0f, 1.0f, 0.0f, 1.0f,
                                           1.0f, 1.0f, 0.0f, 1.0f,
                                           0.0f, 0.0f,-1.0f, 1.0f,
                                           1.0f, 0.0f,-1.0f, 1.0f,
                                           0.0f, 1.0f,-1.0f, 1.0f,
                                           1.0f, 1.0f,-1.0f, 1.0f }};
  std::array<GLfloat,8*4> colorsArray = {{ 0.0f, 0.0f, 1.0f, 1.0f,
                                           0.0f, 0.0f, 1.0f, 1.0f,
                                           0.0f, 0.0f, 1.0f, 1.0f,
                                           0.0f, 0.0f, 1.0f, 1.0f,
                                           0.0f, 0.0f, 1.0f, 1.0f,
                                           0.0f, 0.0f, 1.0f, 1.0f,
                                           0.0f, 0.0f, 1.0f, 1.0f,
                                           0.0f, 0.0f, 1.0f, 1.0f }};
  std::array<GLfloat,8*3> normalArray = {{-1.0f,-1.0f, 1.0f,
                                           1.0f,-1.0f, 1.0f, 
                                          -1.0f, 1.0f, 1.0f, 
                                           1.0f, 1.0f, 1.0f, 
                                          -1.0f,-1.0f,-1.0f, 
                                           1.0f,-1.0f,-1.0f, 
                                          -1.0f, 1.0f,-1.0f, 
                                           1.0f, 1.0f,-1.0f }};
  std::array<GLfloat,8> normDiffArray;
  for(size_t i = 0; i < normalArray.size(); i += 3) {
    gx::vector3 norm(normalArray[i],normalArray[i+1],normalArray[i+2]);
    norm.normalize();
    //normal diff calculatio is just a hack that works for this cube
    normDiffArray[i/3] =((norm - gx::vector3(normalArray[i],  0,0)).magnitude()
                       + (norm - gx::vector3(0,normalArray[i+1],0)).magnitude()
                       + (norm - gx::vector3(0,0,normalArray[i+2])).magnitude())
                       / 3;
    normalArray[i]   = norm.x;
    normalArray[i+1] = norm.y;
    normalArray[i+2] = norm.z;
  }
  std::array<GLuint,6*6>  indicesArray = {{ 0, 1, 2, 1, 3, 2,
                                            2, 3, 6, 3, 7, 6,
                                            4, 0, 6, 0, 2, 6,
                                            1, 5, 3, 5, 7, 3,
                                            4, 5, 0, 5, 1, 0,
                                            5, 4, 7, 4, 6, 7 }};

  std::vector<GLfloat> positions(      posArray.begin(),       posArray.end());
  std::vector<GLfloat> colors(      colorsArray.begin(),    colorsArray.end());
  std::vector<GLfloat> normals(     normalArray.begin(),    normalArray.end());
  std::vector<GLfloat> normDiffs( normDiffArray.begin(),  normDiffArray.end());
  std::vector<GLuint>  indices(    indicesArray.begin(),   indicesArray.end());

  gx::vertexAttrib* positionsAttrib = new gx::vertexAttrib("position",4,0,positions);
  gx::vertexAttrib* colorsAttrib = new    gx::vertexAttrib("color"   ,4,0,colors);
  gx::vertexAttrib* normalsAttrib = new   gx::vertexAttrib("normal"  ,3,0,normals);
  gx::vertexAttrib* normDiffAttrib = new  gx::vertexAttrib("normDiff",1,0,normDiffs);

  std::vector<const gx::vertexAttrib*> attribs;
  attribs.push_back(positionsAttrib);
  attribs.push_back(colorsAttrib);
  attribs.push_back(normalsAttrib);
  attribs.push_back(normDiffAttrib);

  std::vector<gx::drawSet::vaoData_t> entitiesData;
  entitiesData.push_back(std::make_pair(indices,attribs));
  return entitiesData;
}
//must call after window is initialized
GLenum initGlew() {
  //should glew be done per context?? if so, move to static method
  //and ensure that it is only called once
  GLenum glewErr = glewInit();
  if(GLEW_OK != glewErr) {
    std::cout << "error initializing GLEW!" << std::endl;
    exit(1);
  }
  return glewErr;
}
} //end unnamed namespace

void gx::graphicsClient::reshape(unsigned int w, unsigned int h) {
  typedef displaySet::elem_t elem_t;
  const elem_t fov       = 53.13f;
  const elem_t ratio     = elem_t(w) / elem_t(h);
  const elem_t nearPlane = 1.0f;
  const elem_t farPlane  = 30.0f;
  // adjust the viewport when the window is resized
  glViewport(0, 0, w, h);
  gx::debugout << "glViewport(0, 0, w, h);" << gx::endl;
  this->display.setProjection(fov,ratio,nearPlane,farPlane);
}

std::vector<const gx::uniform*> gx::graphicsClient::uniforms() {
  std::vector<const gx::uniform*> ret;
  ret.push_back(&this->display.storage());
  ret.push_back(&this->light1.storage());
  return ret;
}
// create the window
//make sure this is above all opengl objects so that the desctructor is called
//last so we have an opengl context for destructors
gx::graphicsClient::graphicsClient():
    window(sf::VideoMode(defaultWindowWidth, defaultWindowHeight),
           "DrChao", sf::Style::Default),
    glewStatus(initGlew()), //glew needs to be called here, after window, before anything else
    light1(gx::vector4(1,1,1),0.5,0.5,0.05f),
    display(),
    entities(readFile("graphics/default.vert"),readFile("graphics/default.frag"),
                       entitiesData(),uniforms()),
    fpsClock(), fpsFrames(0) {
  this->window.setVerticalSyncEnabled(false);
  this->window.setMouseCursorVisible(false);
  if(!this->window.setActive()) {
    std::cout << "error activating window" << std::endl;
    exit(1);
  }
  
  glEnable(GL_DEPTH_TEST); //enable depth buffer
  glClearColor(1.0,1.0,1.0,1.0); //start with a white screen
  glClearDepth(1.f);
  glDepthFunc(GL_LEQUAL);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);

  light1.updatePosition(gx::vector4( 0, 5, -10));

  setCamera(display);
  setUpMouse();
  this->reshape(defaultWindowWidth, defaultWindowHeight);
}

gx::userInput gx::graphicsClient::handleInput() {
  bool stopped = false;
  userInput curInput;
  sf::Event event;
  while (window.pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      stopped = true; // end the program
    } else if (event.type == sf::Event::Resized) {
      reshape(event.size.width, event.size.height);
    } else if (event.type == sf::Event::KeyPressed) {
      if(event.key.code == sf::Keyboard::Escape) {
        stopped = true; // end the program
	  } else if(event.key.code == sf::Keyboard::Space) {
        curInput.jumped = true;
	  }
    }
  }
  curInput.move = movePlayer(display);
  curInput.stopped = stopped;
  turnPlayer(display);
  return curInput;
}

void gx::graphicsClient::draw() {
      // clear the buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    gx::debugout << "glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT";
    gx::debugout << "| GL_STENCIL_BUFFER_BIT);" << gx::endl;

    // draw...
	entities.draw(display.projection * display.view);

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

void gx::graphicsClient::updateEntities(std::vector<std::pair<vector3,int>> data) {
  this->entities.reset();

  for(auto entityP = data.begin(); entityP != data.end(); ++entityP) {
    const auto& entity = *entityP;
    entities.addEntity(entity.first, entity.second);
  }
}