#pragma once

#include <LinearMath/btIDebugDraw.h>

class BulletDebugDrawer : public btIDebugDraw
{
public:
	//virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColour, const btVector3& toColour);

	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& colour);

	virtual void drawContactPoint(const btVector3 &pointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color);

	virtual void reportErrorWarning(const char* warningString);

	virtual void draw3dText(const btVector3 &location, const char *textString);

	virtual void setDebugMode(int inDebugMode);

	virtual int getDebugMode() const;

private:

	int debugMode;
};