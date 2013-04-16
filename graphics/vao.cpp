#include "vao.h"

gx::vao::vao(std::vector<GLuint> indices, std::vector<vertexAttrib> attribs)
       : id(), numIndices(indices.size()) {
  debugout << "glGenVertexArrays(1, &(this->id));" << endl;
  glGenVertexArrays(1, &(this->id));
  debugout << "glBindVertexArray(" << this->id << ");" << endl;
  glBindVertexArray(this->id);

  GLuint ibo;

  debugout << "glGenBuffers(1, &ibo);" << endl;
  glGenBuffers(1, &ibo);
  debugout << "glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, " << ibo << ");" << endl;
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  debugout << "glBufferData(GL_ELEMENT_ARRAY_BUFFER, ";
  debugout << indices.size() * sizeof(GLuint);
  debugout << ", indices.data(), GL_STATIC_DRAW);" << endl;
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
                                        indices.data(), GL_STATIC_DRAW);

  for(const auto& attrib : attribs) {
    attrib.bindBuffer();
    debugout << "glEnableVertexAttribArray(" << attrib.loc() << ");" << endl;
    glEnableVertexAttribArray(attrib.loc());
    debugout << "glVertexAttribPointer(" << attrib.loc() << ", ";
    debugout << attrib.vecSize() << ", attrib.type(), GL_FALSE, ";
    debugout << attrib.stride() << ", 0);" << endl;
    glVertexAttribPointer(attrib.loc(), attrib.vecSize(), attrib.type(),
                          GL_FALSE, attrib.stride(), 0);
  }
}

void gx::vao::draw() {
  debugout << "glBindVertexArray(" << this->id << ");" << endl;
  glBindVertexArray(this->id);
  debugout << "glDrawElements(GL_TRIANGLES, " << this->numIndices;
  debugout << ", GL_UNSIGNED_INT, nullptr);" << endl;
  glDrawElements(GL_TRIANGLES, this->numIndices, GL_UNSIGNED_INT, nullptr);
}
