#pragma once

#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>

class DebugPrimitives
{
public:

	struct VertexColour
	{
		glm::vec3 v;
		glm::vec3 c;
	};

	DebugPrimitives(GLenum inPrimitiveType);

	~DebugPrimitives();

	void addData(glm::vec3 newData);

	void populateVBO();

	void render();

	void clearData();
protected:
	GLenum primitiveType;

	GLuint VAO;

	/** @brief Buffer for vertex data. */
	GLuint VBO;

	std::vector<glm::vec3> data;
};