#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "ContactPoint.h"
#include "Line.h"
#include "DebugPrimitives.h"
#include "../../Shader.h"

/** @brief	A debug drawer. */
class DebugDrawer
{
public:

	/**
	 @brief	Initialise.
	
	 @param	vShaderFilename	Filename of the vertex shader file.
	 @param	fShaderFilename	Filename of the fragment shader file.
	 */
	static void init(std::string vShaderFilename, std::string fShaderFilename);

	/** @brief	Clean-ups this DebugDrawer. */
	static void cleanup();

	/** @brief	Pre render. */
	static void preRender();

	/** @brief	Renders all elements in the Debug Drawer. */
	static void render();

	/** @brief	Post render. */
	static void postRender();

	/**
	 @brief	Adds a line.
	
	 @param	pA	  	The point a.
	 @param	pB	  	The point b.
	 @param	colour	The colour.
	 */
	static void addLine(glm::vec3 pA, glm::vec3 pB, glm::vec3 colour);

	/**
	 @brief	Adds a line.
	
	 @param	pA	  	The point a.
	 @param	pB	  	The point b.
	 @param	pAColour	point a colour.
	 @param	pBColour	point b colour.
	 */
	static void addLine(glm::vec3 pA, glm::vec3 pB, glm::vec3 pAColour, glm::vec3 pBColour);

	/**
	 @brief	Adds a line.
	
	 @param	line	The line.
	 */
	static void addLine(Line line);

	/**
	 @brief	Adds a contact point.
	
	 @param	point 	The point.
	 @param	normal	The normal.
	 @param	colour	The colour.
	 */
	static void addContactPoint(glm::vec3 point, glm::vec3 normal, glm::vec3 colour);

	/**
	 @brief	Adds a contact point.
	
	 @param	point	The point.
	 */
	static void addContactPoint(ContactPoint point);

	/**
	 @brief	Sets render point size.
	
	 @param	pointSize	Size of the point.
	 */
	static void setPointSize(float pointSize);

	/**
	 @brief	Sets render line size.
	
	 @param	lineSize	Size of the line.
	 */
	static void setLineSize(float lineSize);

	/**
	 @brief	Toggles whether the wire frame is visible through other models.
	 */
	static void toggleZeroDepthWireFrame();
private:

	/** @brief	The lines. */
	static std::shared_ptr<DebugPrimitives> lines;
	/** @brief	The points. */
	static std::shared_ptr<DebugPrimitives> points;

	/** @brief	The shader. */
	static std::shared_ptr<Shader> shader;

	/** @brief	True to use zero depth wire frame. */
	static bool zeroDepthWireFrame;
};