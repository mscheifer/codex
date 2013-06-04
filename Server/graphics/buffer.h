#include <GL/glew.h>
#include <vector>
#include "oglUtil.h"

namespace gx {
class buffer {
    GLuint name;
    GLenum target;
  public:
    template<typename T>
    buffer(GLenum t,const std::vector<T>& d, GLenum hint): name(), target(t) {
      glGenBuffers(1, &(this->name));
      debugout << "glGenBuffers(1, &buffer@" << &(this->name) << ");" << endl;
      this->bind();
      glBufferData(target, d.size() * sizeof(T), d.data(), hint);
      debugout << "glBufferData(" << target << ", " << d.size() * sizeof(T);
      debugout << ", d.data(), " << hint << ");" << endl;
    }
    buffer(buffer const&);//= delete; //don't copy
    buffer& operator=(buffer const&);//= delete; //don't copy
    buffer(buffer&&) noexcept;
    buffer& operator=(buffer&&);//= delete; //define later
    ~buffer() noexcept;
    void bind() const;
    template<typename T>
    void write(const std::vector<T>& d) {
      this->bind();
      glBufferSubData(this->target,0,d.size() * sizeof(T), d.data());
      debugout << "glBufferSubData(" << this->target << ",0,";
      debugout << d.size() * sizeof(T) << ", " << d.data() << ");" << endl;
    }
};
} //end namespace gxz