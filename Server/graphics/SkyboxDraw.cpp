#include "SkyboxDraw.h"
#include <array>
#include "uniformBlock.h"

namespace {
	const std::string vertShaderName = readFile("shaders/skybox.vert");
	const std::string fragShaderName = readFile("shaders/skybox.frag");
	const unsigned int textureUnit = 0;

	std::vector<GLuint> initIndices() {
		std::array<GLuint,5*6> skyboxIndices = {{ 0, 2, 1, 0, 3, 2,
                                            1, 5, 0, 0, 5, 4,
                                            0, 7, 3, 0, 4, 7,
                                            2, 6, 1, 6, 5, 1,
                                            3, 6, 2, 3, 7, 6}};
		
		return std::vector<GLuint>(skyboxIndices.begin(), skyboxIndices.end());
	}

	gx::vao::attribsList_t initAttrib() {
		std::array<GLfloat,8*4> skyboxVtArr = {{  1000.0f, 1000.0f, 500.0f, 1.0f,
                                          -1000.0f, 1000.0f, 500.0f, 1.0f,
                                          -1000.0f,-1000.0f, 500.0f, 1.0f,
                                           1000.0f,-1000.0f, 500.0f, 1.0f,
                                           1000.0f, 1000.0f,   0.0f, 1.0f,
                                          -1000.0f, 1000.0f,   0.0f, 1.0f,
                                          -1000.0f,-1000.0f,   0.0f, 1.0f,
                                           1000.0f,-1000.0f,   0.0f, 1.0f }};

		std::vector<GLfloat> positions(skyboxVtArr.begin(), skyboxVtArr.end());
		auto posAttrib = std::make_shared<gx::vertexAttrib>("position",4,0,positions);
		gx::vao::attribsList_t ret;
		ret.push_back(std::move(posAttrib));

		return ret;
	}
}

gx::SkyboxDraw::SkyboxDraw(uniform::block & VP) 
	: program(vertShaderName, fragShaderName, std::vector<uniform::block*>(1,&VP)),
	drawData(initIndices(), initAttrib(), program.vars()), texture(),
	VPStorage(VP)
{
	GLint samplerLoc = program.uniformLoc("gCubemapTexture");
	program.use();
	glUniform1i(samplerLoc, textureUnit);

}

void gx::SkyboxDraw::draw() {
	program.use();
	this->VPStorage.frameUpdate(&(this->program));
	texture.Bind(GL_TEXTURE0 + textureUnit);
	drawData.draw();
}