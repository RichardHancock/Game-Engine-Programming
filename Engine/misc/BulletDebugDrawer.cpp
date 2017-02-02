#include "BulletDebugDrawer.h"

#include <GL\glew.h>

#include "Log.h"

/*
void BulletDebugDrawer::drawLine(const btVector3 & from, const btVector3 & to, const btVector3 & fromColour, const btVector3 & toColour)
{
	glPushMatrix();
	glColor4f(fromColour.getX(), fromColour.getY(), fromColour.getZ(), 1.0f);


}*/

void BulletDebugDrawer::drawLine(const btVector3 & from, const btVector3 & to, const btVector3 & colour)
{
	/*
	glPushMatrix();
	glColor4f(colour.getX(), colour.getY(), colour.getZ(), 1.0f);

	const GLfloat line[] = {
		from.getX(), from.getY(), from.getZ(),
		to.getX(),   to.getY(),   to.getZ()
	};

	glVertexPointer(3, GL_FLOAT, 0, &line);

	glPointSize(10.0f);

	glDrawArrays(GL_POINTS, 0, 2);
	glDrawArrays(GL_LINES, 0, 2);

	glPopMatrix();*/

	glColor4f(colour.getX(), colour.getY(), colour.getZ(), 1.0f);
	glBegin(GL_LINES);

	glVertex3f(from.getX(), from.getY(), from.getZ());
	glVertex3f(to.getX(), to.getY(), to.getZ());

	glEnd();
}

void BulletDebugDrawer::drawContactPoint(const btVector3 & pointOnB, const btVector3 & normalOnB, btScalar distance, int lifeTime, const btVector3 & color)
{
}

void BulletDebugDrawer::reportErrorWarning(const char * warningString)
{
	Log::logW(warningString);
}

void BulletDebugDrawer::draw3dText(const btVector3 & location, const char * textString)
{
}

void BulletDebugDrawer::setDebugMode(int inDebugMode)
{
	debugMode = inDebugMode;
}

int BulletDebugDrawer::getDebugMode() const
{
	return debugMode;
}

