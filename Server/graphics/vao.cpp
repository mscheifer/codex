#include "vao.h"
#include "vertexAttrib.h"

gx::vao::vao(const std::vector<GLuint> indices, attribsList_t attrs,
             varSigs_t sigs)
       : id(), numIndices(static_cast<GLsizei>(indices.size())), ibo(),
         attribs(std::move(attrs)) {
  glGenVertexArrays(1, &(this->id));
  debugout << "glGenVertexArrays(1, &(this->id));" << endl;
  glBindVertexArray(this->id);
  debugout << "glBindVertexArray(" << this->id << ");" << endl;

  glGenBuffers(1, &(this->ibo));
  debugout << "glGenBuffers(1, &(this->ibo));" << endl;
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
  debugout << "glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, " << this->ibo << ");\n";
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
                                        indices.data(), GL_STATIC_DRAW);
  debugout << "glBufferData(GL_ELEMENT_ARRAY_BUFFER, ";
  debugout << indices.size() * sizeof(GLuint);
  debugout << ", indices.data(), GL_STATIC_DRAW);" << endl;

  for(auto attribp = this->attribs.begin(); attribp != this->attribs.end();
                                                           ++attribp) {
    const auto& attrib = **attribp;
    auto itPos = sigs.find(attrib.name());
    if(itPos != sigs.end()) {
      if(!itPos->second.checkAndBind(attrib)) {
        std::cout << "error: shader variable: " << attrib.name();
        std::cout << " does not match type or vector size" << std::endl;
      }
      sigs.erase(itPos);
    } else {
      std::cout << "error: no variable in shader named: " << attrib.name();
      std::cout << std::endl;
    }
  }
  if(!sigs.empty()) {
    std::cout << "Error: no data for shader variables: " << std::endl;
    for(auto sigp = sigs.begin(); sigp != sigs.end(); ++sigp) {
      const auto& s = *sigp;
      std::cout << s.first << std::endl;
    }
  }
}

gx::vao::vao(vao&& other): id(other.id), numIndices(other.numIndices),
                           ibo(other.ibo), attribs(std::move(other.attribs)) {
  other.id  = 0; //delete vertex array won't complain
  other.ibo = 0; //delete buffers won't complain
}

gx::vao& gx::vao::operator=(vao&& other) {
  this->id  = other.id;
  this->ibo = other.ibo;
  other.id  = 0; //delete vertex array won't complain
  other.ibo = 0; //delete buffers won't complain
  return *this;
}

gx::vao::~vao() {
  glDeleteBuffers(1, &(this->ibo));
  debugout << "glDeleteBuffers(1, &(this->ibo): " << this->ibo << ");\n";
  glDeleteVertexArrays(1, &(this->id));
  debugout << "glDeleteVertexArrays(1, &(this->id): " << this->id << ");\n";
}

void gx::vao::drawOnce() const {
  this->drawHead();
  this->drawInstance();
}

void gx::vao::drawHead() const {
  glBindVertexArray(this->id);
  debugout << "glBindVertexArray(" << this->id << ");" << endl;
}

void gx::vao::drawInstance() const {
  this->drawInstance(this->numIndices);
}

void gx::vao::drawInstance(GLsizei num) const {
  glDrawElements(GL_TRIANGLES, num, GL_UNSIGNED_INT, nullptr);
  debugout << "glDrawElements(GL_TRIANGLES, " << num;
  debugout << ", GL_UNSIGNED_INT, nullptr);" << endl;
}