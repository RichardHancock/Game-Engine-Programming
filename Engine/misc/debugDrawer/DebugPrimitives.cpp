#include "DebugPrimitives.h"

#include "../../Platform.h"

DebugPrimitives::DebugPrimitives(GLenum inPrimitiveType)
{
	primitiveType = inPrimitiveType;
	VBO = 0;
	
	if (!Platform::isDummyRenderer())
	{
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		//Positions
		glVertexAttribPointer(
			0,									    //Attribute ID
			3,									    //Size
			GL_FLOAT,							    //Type
			GL_FALSE,							    //Normalized?
			sizeof(VertexColour),				    //Stride
			(void*)(0 + offsetof(VertexColour, v))	//Array Buffer Offset
		);

		//Colours
		glVertexAttribPointer(
			1,										//Attribute ID
			3,										//Size
			GL_FLOAT,								//Type
			GL_FALSE,								//Normalized?
			sizeof(VertexColour),					//Stride
			(void*)(0 + offsetof(VertexColour, c))	//Array Buffer Offset
		);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

DebugPrimitives::~DebugPrimitives()
{
	if (VBO != 0)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}

	if (VAO != 0)
	{
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}

	data.clear();
}

void DebugPrimitives::addData(glm::vec3 newData)
{
	data.push_back(newData);
}

void DebugPrimitives::populateVBO()
{
	if (data.size() == 0)
		return;

	//Data size should always be even (1 position, 1 colour)
	assert(data.size() % 2 == 0);

	if (VAO != 0)
	{
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * data.size(), &data[0], GL_STREAM_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		//Positions
		glVertexAttribPointer(
			0,									    //Attribute ID
			3,									    //Size
			GL_FLOAT,							    //Type
			GL_FALSE,							    //Normalized?
			sizeof(glm::vec3) * 2,				    //Stride
			(void*)(0)	//Array Buffer Offset
		);

		//Colours
		glVertexAttribPointer(
			1,										//Attribute ID
			3,										//Size
			GL_FLOAT,								//Type
			GL_FALSE,								//Normalized?
			sizeof(glm::vec3) * 2,					//Stride
			(void*)(sizeof(glm::vec3))	//Array Buffer Offset
		);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void DebugPrimitives::render()
{
	if (VAO != 0 && data.size() > 0)
	{
		glBindVertexArray(VAO);

		glDrawArrays(primitiveType, 0, data.size() / 2);

		glBindVertexArray(0);
	}
}

void DebugPrimitives::clearData()
{
	data.clear();
}
