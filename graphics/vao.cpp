#include "vao.h"

gx::vao::vao(const std::vector<GLuint>                   indices,
             const std::vector<const vertexAttrib*>      attribs,
             std::map<std::string,vertexAttribSignature> sigs)
       : id(), numIndices(indices.size()), ibo() {
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

  //cant use range based for here because of visual c++
  for(auto attribp = attribs.begin() ; attribp != attribs.end() ; ++attribp) {
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
	for(auto sigp = sigs.begin(); sigp != sigs.end(); ++sigp){
	  const auto& s = *sigp;
      std::cout << s.first << std::endl;
    }
  }
}

gx::vao::vao(vao&& other): id(other.id), numIndices(other.numIndices),
                           ibo(other.ibo) {
  other.id  = 0; //delete vertex array won't complain
  other.ibo = 0; //dlete buffers won't complain
}

gx::vao::~vao() {
  glDeleteBuffers(1, &(this->ibo));
  debugout << "glDeleteBuffers(1, &(this->ibo)@" << &(this->ibo) << ");\n";
  glDeleteVertexArrays(1, &(this->id));
  debugout << "glDeleteVertexArrays(1, &(this->id)@" << &(this->id) << ");\n";
}

void gx::vao::draw() const {
  glBindVertexArray(this->id);
  debugout << "glBindVertexArray(" << this->id << ");" << endl;
  glDrawElements(GL_TRIANGLES, this->numIndices, GL_UNSIGNED_INT, nullptr);
  debugout << "glDrawElements(GL_TRIANGLES, " << this->numIndices;
  debugout << ", GL_UNSIGNED_INT, nullptr);" << endl;
}
