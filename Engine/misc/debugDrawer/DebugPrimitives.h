#pragma once

#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>

/** @brief	A debug primitives renderer responsible for one type of primitive. */
class DebugPrimitives
{
public:

	/** @brief	A vertex colour. */
	struct VertexColour
	{
		glm::vec3 v;
		glm::vec3 c;
	};

	/**
	 @brief	Constructor.
	
	 @param	inPrimitiveType	Type of the primitive.
	 */
	DebugPrimitives(GLenum inPrimitiveType);

	/** @brief	Destructor. */
	~DebugPrimitives();

	/**
	 @brief	Adds a new vec3.
	
	 @param	newData	A Vec3.
	 */
	void addData(glm::vec3 newData);

	/** @brief	Populate VBO with stored data. */
	void populateVBO();

	/** @brief	Renders this DebugPrimitives. */
	void render();

	/** @brief	Clears the data. */
	void clearData();
protected:
	/** @brief	Type of the primitive. */
	GLenum primitiveType;

	/** @brief	The vao. */
	GLuint VAO;

	/** @brief Buffer for vertex data. */
	GLuint VBO;

	/** @brief	The data array. */
	std::vector<glm::vec3> data;
};