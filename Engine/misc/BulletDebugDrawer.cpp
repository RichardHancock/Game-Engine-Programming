#include "BulletDebugDrawer.h"

#include <GL/glew.h>

#include "Log.h"
#include "Utility.h"
#include "debugDrawer/DebugDrawer.h"


void BulletDebugDrawer::drawLine(const btVector3 & from, const btVector3 & to, const btVector3 & fromColour, const btVector3 & toColour)
{
	DebugDrawer::addLine(
		Utility::bulletVec3ToGLM(from),
		Utility::bulletVec3ToGLM(to),
		Utility::bulletVec3ToGLM(fromColour),
		Utility::bulletVec3ToGLM(toColour)
	);
}

void BulletDebugDrawer::drawLine(const btVector3 & from, const btVector3 & to, const btVector3 & colour)
{
	DebugDrawer::addLine(
		Utility::bulletVec3ToGLM(from),
		Utility::bulletVec3ToGLM(to),
		Utility::bulletVec3ToGLM(colour)
	);
}

void BulletDebugDrawer::drawContactPoint(const btVector3 & pointOnB, const btVector3 & normalOnB, btScalar, int, const btVector3 & colour)
{  
	DebugDrawer::addContactPoint(
		Utility::bulletVec3ToGLM(pointOnB), 
		Utility::bulletVec3ToGLM(normalOnB),
		Utility::bulletVec3ToGLM(colour)
	);
}

void BulletDebugDrawer::reportErrorWarning(const char * warningString)
{
	Log::logW(warningString);
}

void BulletDebugDrawer::draw3dText(const btVector3 & location, const char * textString)
{
	Log::logD("Text that should be rendered at Vec3(" +
		Utility::floatToString(location.getX()) +
		Utility::floatToString(location.getY()) +
		Utility::floatToString(location.getZ()) +
		"): " + std::string(textString));
}

void BulletDebugDrawer::setDebugMode(int inDebugMode)
{
	debugMode = inDebugMode;
}

int BulletDebugDrawer::getDebugMode() const
{
	return debugMode;
}

