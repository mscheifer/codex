#include "displaySet.h"

namespace { //don't export
 const std::string uniformName = "display";
 const std::array<gx::uniform::sigs_t,2> uniformVarsArray = {{ 
   std::make_pair("viewMatrix",std::make_pair(GL_FLOAT_MAT4,1)), 
   std::make_pair("projMatrix",std::make_pair(GL_FLOAT_MAT4,1))
 }};
 const std::map<gx::uniform::name_t,gx::uniform::type_t>
   uniformVars(uniformVarsArray.begin(),uniformVarsArray.end());
} //end unnamed namespace

gx::displaySet::displaySet(): view(), projection(), cameraPos(),
    unif(uniformName,uniformVars) {}

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

  this->storage().write("projMatrix",this->projection.oglmatrix());
}
//e is camera position and d is look at point
void gx::displaySet::setView(const vector4f& e, const vector4f& d, 
                             const vector3f& up) {
  vector3f zc, xc, yc;
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
  this->storage().write("viewMatrix",this->view.oglmatrix());

  this->cameraPos = e;
  //TODO: add write for camera pos
}

gx::uniform::block& gx::displaySet::storage() {
  return this->unif;
}
