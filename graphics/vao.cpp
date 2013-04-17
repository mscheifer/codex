#include "vao.h"

gx::vao::vao(std::vector<GLuint> indices, std::vector<vertexAttrib> attribs)
       : id(), numIndices(indices.size()) {
  glGenVertexArrays(1, &(this->id));
  debugout << "glGenVertexArrays(1, &(this->id));" << endl;
  glBindVertexArray(this->id);
  debugout << "glBindVertexArray(" << this->id << ");" << endl;

  GLuint ibo;

  glGenBuffers(1, &ibo);
  debugout << "glGenBuffers(1, &ibo);" << endl;
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  debugout << "glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, " << ibo << ");" << endl;
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
                                        indices.data(), GL_STATIC_DRAW);
  debugout << "glBufferData(GL_ELEMENT_ARRAY_BUFFER, ";
  debugout << indices.size() * sizeof(GLuint);
  debugout << ", indices.data(), GL_STATIC_DRAW);" << endl;

  //cant use range based for here because of visual c++
  for(auto attribp = attribs.begin() ; attribp != attribs.end() ; ++attribp) {
	const auto& attrib = *attribp;
    attrib.bindBuffer();
    glEnableVertexAttribArray(attrib.loc());
    debugout << "glEnableVertexAttribArray(" << attrib.loc() << ");" << endl;
    glVertexAttribPointer(attrib.loc(), attrib.vecSize(), attrib.type(),
                          GL_FALSE, attrib.stride(), 0);
    debugout << "glVertexAttribPointer(" << attrib.loc() << ", ";
    debugout << attrib.vecSize() << ", attrib.type(), GL_FALSE, ";
    debugout << attrib.stride() << ", 0);" << endl;
  }
}

void gx::vao::draw() {
  glBindVertexArray(this->id);
  debugout << "glBindVertexArray(" << this->id << ");" << endl;
  glDrawElements(GL_TRIANGLES, this->numIndices, GL_UNSIGNED_INT, nullptr);
  debugout << "glDrawElements(GL_TRIANGLES, " << this->numIndices;
  debugout << ", GL_UNSIGNED_INT, nullptr);" << endl;
}
