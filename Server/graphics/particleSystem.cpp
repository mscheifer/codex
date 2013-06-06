#include "particleSystem.h"
#include <algorithm>
#include "vector3.h"

const std::string gx::particleDrawerImpl::shaderID = "particle";

const std::string gx::particleDrawerImpl::vertShader =
  readFile("shaders/particle.vert");
const std::string gx::particleDrawerImpl::fragShader =
  readFile("shaders/particle.frag");

namespace {
  const std::string positionVarName = "position";
  const std::string texCoordsVarName = "texCoord";
  const std::string colVarName = "color";

  std::vector<GLuint> initIndices() {
    std::vector<GLuint> ret;
    std::array<GLuint,6> indexPattern = {{ 0, 1, 2, 0, 2, 3 }};
    for(unsigned int i = 0; i < gx::maxParticles; i++) {
      std::transform(indexPattern.begin(), indexPattern.end(),
        indexPattern.begin(), [] (GLuint a) { return a + 4; });
      ret.insert(ret.end(), indexPattern.begin(), indexPattern.end());
    }
    return ret;
  }

  gx::particleEntity::dynamicPtr_t initPosAttrib() {
    std::vector<GLfloat> dummyData;
    dummyData.resize(4 * 4 * gx::maxParticles); //4 vec4s
    auto ret = std::make_shared<gx::dynamicVertexAttrib>
      (positionVarName,4,0,std::move(dummyData));
    return std::move(ret);
  }
  
  gx::particleEntity::dynamicPtr_t initColAttrib() {
    std::vector<GLfloat> dummyData;
    dummyData.resize(4 * 4 * gx::maxParticles); //4 vec4s
    auto ret = std::make_shared<gx::dynamicVertexAttrib>
      (colVarName,4,0,std::move(dummyData));
    return std::move(ret);
  }

  gx::particleEntity::attribsList_t
  baseList(gx::particleEntity::dynamicPtr_t a, 
           gx::particleEntity::dynamicPtr_t b) 
  {
    std::vector<GLfloat> texCoordsData;
	//fix the texcoords if the texture is fixed
	std::array<GLfloat,4*2> texCoordsPattern = {{ 0.9f, 0.9f, 0.9f, 0.1f,
	                                              0.1f, 0.1f, 0.1f, 0.9f}};
    texCoordsData.reserve(4 * 2 * gx::maxParticles); //4 vec2s
	for(unsigned int i = 0; i < gx::maxParticles; i++) {
      texCoordsData.insert(texCoordsData.end(), texCoordsPattern.begin(), texCoordsPattern.end());
    }
    auto texCoords = std::make_shared<gx::dynamicVertexAttrib>
      (texCoordsVarName,2,0,std::move(texCoordsData));

    gx::particleEntity::attribsList_t ret;
    ret.push_back(std::dynamic_pointer_cast<const gx::vertexAttrib>(a));
    ret.push_back(std::dynamic_pointer_cast<const gx::vertexAttrib>(b));
    ret.push_back(std::move(texCoords));
    return std::move(ret);
  }
}

gx::particleEntity::particleEntity(material pmat)
  : indices(initIndices()),
    positionAttrib(initPosAttrib()),
    colorAttrib(initColAttrib()),
    attribs(baseList(positionAttrib, colorAttrib)),
    mat(std::move(pmat)) {}

gx::particleEntity::particleEntity(particleEntity&& other) noexcept
  : indices(std::move(other.indices)),
    positionAttrib(std::move(other.positionAttrib)),
    colorAttrib(std::move(other.colorAttrib)),
    attribs(std::move(other.attribs)),
    mat(std::move(other.mat)) {}

gx::particleEntity&
gx::particleEntity::operator=(particleEntity&&) {
  assert(false);
  return *this;
}

gx::particleDrawerImpl::particle::particle(vector4f pos, vector3f vel)
  : position(pos), velocity(vel), lifetime(0) {}

gx::particleDrawerImpl::entityClass::entityClass(entity_t drawData,
                                                 varSigs_t vars)
  : positionAttrib(std::move(drawData.positionAttrib)),
    colorAttrib(std::move(drawData.colorAttrib)), numIndices(0),
    vertData(std::move(drawData.indices), std::move(drawData.attribs),
    std::move(vars)), mat(std::move(drawData.mat)) {}

gx::particleDrawerImpl::entityClass::entityClass(entityClass&& other) noexcept
  : instances(std::move(other.instances)), particles(std::move(other.particles)),
    positionAttrib(std::move(other.positionAttrib)),
    colorAttrib(std::move(other.colorAttrib)), 
    numIndices(other.numIndices), vertData(std::move(other.vertData)),
    mat(std::move(other.mat)) {}

gx::particleDrawerImpl::entityClass&
gx::particleDrawerImpl::entityClass::operator=(entityClass&&) {
  assert(false);
  return *this;
}

void gx::particleDrawerImpl::entityClass::clear() {
  //do nothing
}

namespace {
  std::vector<GLfloat> positions;
  std::vector<GLfloat> colors;

  float randGenf(float min, float max) {
    const unsigned int randomPrecision = 1000;
    auto random = (rand() % randomPrecision) / static_cast<float>(randomPrecision);
    float diff = max - min;
    float r = random * diff;
    return min + r;
  }
} //end unnamed namespace

void gx::particleDrawerImpl::entityClass::addParticle(const vector4f& pos) {
  for(unsigned int i = 0; i < particlesPerFrame; i++) {
    auto randX = randGenf(-10,10) * randGenf(0.00005f, 0.0001f);
    auto randY = randGenf(-10,10) * randGenf(0.00005f, 0.0001f);
    particle p(pos, vector3f(randX, randY, 0.0f));
	  this->particles.push_back(std::move(p));
  }
  this->numIndices += 6 * particlesPerFrame;
}

void gx::particleDrawerImpl::entityClass::update() {
  for(auto itr = this->realInstances.begin(); itr != this->realInstances.end(); itr++) {
    this->addParticle(*itr);
  }
  this->realInstances.clear(); //only draw the single static instance once
  for(auto itr = this->realStaticInstances.begin(); itr != this->realStaticInstances.end(); itr++) {
    this->addParticle(*itr);
  }

  positions.clear();
  positions.reserve(particles.size() * 4 * 4); //could move this call to only run for maxParticles at beginning
  colors.clear();
  colors.reserve(particles.size() * 4 * 4); //could move this call to only run for maxParticles at beginning
  for(auto itr = this->particles.begin(); itr != this->particles.end();) {
    vector3f acceleration(0,0,randGenf(0.000001f, 0.00001f));
    acceleration += vector3f(-itr->velocity.x * randGenf(0.00001f,0.0001f),
                             -itr->velocity.y * randGenf(0.00001f,0.0001f),0);
    itr->velocity += acceleration;
    itr->position += itr->velocity;
    itr->lifetime++;
    if(itr->lifetime > particlesLifeTime) {
      //remove particle
      std::swap(*itr,this->particles.back());
      this->particles.pop_back();
      numIndices -= 6;
    } else {
      //convert particles to positions
      //not billboarded for now
      const float billboardSize = 0.1f;
      auto pos1 = itr->position + vector3f( billboardSize,0, billboardSize);
      auto pos2 = itr->position + vector3f(-billboardSize,0, billboardSize);
      auto pos3 = itr->position + vector3f(-billboardSize,0,-billboardSize);
      auto pos4 = itr->position + vector3f( billboardSize,0,-billboardSize);
      positions.insert(positions.end(), pos1.begin(), pos1.end());
      positions.insert(positions.end(), pos2.begin(), pos2.end());
      positions.insert(positions.end(), pos3.begin(), pos3.end());
      positions.insert(positions.end(), pos4.begin(), pos4.end());

      float factor = static_cast<float>(itr->lifetime) / particlesLifeTime;

      vector4f startColor(1, 0.2, 0);
      vector3f endColor(0, 0, 0);
      vector4f currColor = startColor * (1-factor) + endColor * factor;
      colors.insert(colors.end(), currColor.begin(), currColor.end());
      colors.insert(colors.end(), currColor.begin(), currColor.end());
      colors.insert(colors.end(), currColor.begin(), currColor.end());
      colors.insert(colors.end(), currColor.begin(), currColor.end());
      itr++;
    }
  }
  this->positionAttrib->write(positions);
  this->colorAttrib->write(colors);
}

void gx::particleDrawerImpl::entityClass::draw() {
  this->vertData.drawInstance(this->numIndices);
}

gx::particleDrawerImpl::particleDrawerImpl(const shaderProgram& program) {}

void gx::particleDrawerImpl::setUniforms(const entityClass&,
                                         const entityClass::instance&) const{
  //do nothing
}

void gx::particleDrawerImpl::addInstance(const instanceData& data,
                         std::vector<entityClass>& entityClasses) {
  auto type = data.type;
  entityClasses[type].realInstances.push_back(data.position);
}

void gx::particleDrawerImpl::addStaticInstance(const instanceData& data,
                         std::vector<entityClass>& entityClasses) {
  auto type = data.type;
  entityClasses[type].realStaticInstances.push_back(data.position);
}