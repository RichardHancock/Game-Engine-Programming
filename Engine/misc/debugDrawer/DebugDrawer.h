#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "ContactPoint.h"
#include "Line.h"
#include "DebugPrimitives.h"
#include "../../Shader.h"

class DebugDrawer
{
public:

	static void init(std::string vShaderFilename, std::string fShaderFilename);

	static void cleanup();

	static void preRender();

	/** @brief	Renders all elements in the Debug Drawer. */
	static void render();

	static void postRender();
	
	static void addLine(glm::vec3 pA, glm::vec3 pB, glm::vec3 colour);

	static void addLine(glm::vec3 pA, glm::vec3 pB, glm::vec3 pAColour, glm::vec3 pBColour);

	static void addLine(Line line);

	static void addContactPoint(glm::vec3 point, glm::vec3 normal, glm::vec3 colour);

	static void addContactPoint(ContactPoint point);

	static void setPointSize(float pointSize);

	static void setLineSize(float lineSize);

	/**
	 @brief	Toggles whether the wire frame is visible through other models.
	 */
	static void toggleZeroDepthWireFrame();
private:

	static std::shared_ptr<DebugPrimitives> lines;
	static std::shared_ptr<DebugPrimitives> points;

	static std::shared_ptr<Shader> shader;

	static bool zeroDepthWireFrame;
};