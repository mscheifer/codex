#include "displaySet.h"

gx::displaySet::displaySet(GLuint bindingPoint)
  : view(), projection(), bindingIndex(bindingPoint), bufferName() {
  debugout << "glGenBuffers(1, &(this->bufferName));" << endl;
  glGenBuffers(1, &(this->bufferName));
  debugout << "glBindBuffer(GL_UNIFORM_BUFFER, " << this->bufferName << ");";
  debugout << endl;
  glBindBuffer(GL_UNIFORM_BUFFER, this->bufferName);
  debugout << "glBufferData(GL_UNIFORM_BUFFER, ";
  debugout << sizeof(gx::matrix::elem_t[16]) * 2 << ", nullptr, ";
  debugout << "GL_DYNAMIC_DRAW);" << endl;
  glBufferData(GL_UNIFORM_BUFFER, sizeof(gx::matrix::elem_t[16]) * 2, nullptr,
               GL_DYNAMIC_DRAW);
  debugout << "glBindBufferBase(GL_UNIFORM_BUFFER, " << this->bindingIndex;
  debugout << ", " << this->bufferName << ");" << endl;
  glBindBufferBase(GL_UNIFORM_BUFFER, this->bindingIndex, this->bufferName);
}

void gx::displaySet::setProjection(elem_t fov, elem_t ratio, elem_t nearP,
                               elem_t farP) {
  elem_t f = 1.0f / elem_t(tan (fov * (M_PI / 360.0)));

  this->projection = matrix(); //0 init

  this->projection[0][0] = f / ratio;
  this->projection[1][1] = f;
  this->projection[2][2] = (farP + nearP) / (nearP - farP);
  this->projection[2][3] = (2.0f * farP * nearP) / (nearP - farP);
  this->projection[3][2] = -1.0f;
  this->projection[3][3] = 0.0f;

  auto oglM = this->projection.oglmatrix();

  debugout << "glBindBuffer(GL_UNIFORM_BUFFER, " << this->bufferName << ");";
  debugout << endl;
  glBindBuffer(GL_UNIFORM_BUFFER, this->bufferName);
  debugout << "glBufferSubData(GL_UNIFORM_BUFFER, " << sizeof(oglM) << ", ";
  debugout << sizeof(oglM) << ", oglM.data());" << endl;
  glBufferSubData(GL_UNIFORM_BUFFER, sizeof(oglM), sizeof(oglM), oglM.data());
}
//e is camera position and d is look at point
void gx::displaySet::setView(const vector3& e, const vector3& d, 
                             const vector3& up) {
  vector3 zc, xc, yc;
  zc = e - d;
  zc.normalize();
  xc.cross(up,zc);
  xc.normalize();
  yc.cross(zc,xc);

  matrix r;

  //insert as transpose for inverse
  r[0][0] = xc[0];
  r[0][1] = xc[1];
  r[0][2] = xc[2];
  r[0][3] = 0;
  r[1][0] = yc[0];
  r[1][1] = yc[1];
  r[1][2] = yc[2];
  r[1][3] = 0;
  r[2][0] = zc[0];
  r[2][1] = zc[1];
  r[2][2] = zc[2];
  r[2][3] = 0;
  r[3][0] = 0;
  r[3][1] = 0;
  r[3][2] = 0;
  r[3][3] = 1;

  //insert as negative for inverse
  matrix t = translation(-e[0],-e[1],-e[2]);

  this->view = r * t;

  auto oglM = this->view.oglmatrix();

  debugout << "glBindBuffer(GL_UNIFORM_BUFFER, " << this->bufferName << ");";
  debugout << endl;
  glBindBuffer(GL_UNIFORM_BUFFER, this->bufferName);
  debugout << "glBufferSubData(GL_UNIFORM_BUFFER, 0, " << sizeof(oglM);
  debugout << ", oglM.data());" << endl;
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(oglM), oglM.data());
}

GLuint gx::displaySet::bindPoint() {
  return this->bindingIndex;
}
