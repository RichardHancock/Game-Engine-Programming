#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "ContactPoint.h"
#include "Line.h"

class DebugDrawer
{
public:

	DebugDrawer();

	~DebugDrawer();

	/** @brief	Renders all elements in the Debug Drawer. */
	void render();

	
	void addLine(glm::vec3 pA, glm::vec3 pB, glm::vec3 colour);

	void addLine(Line line);

	void addContactPoint(ContactPoint point);

private:

	std::vector<ContactPoint> points;

	std::vector<Line> lines;
};