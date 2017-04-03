#pragma once

#include "../internal/dependencies/btBulletDynamicsCommon.h"

/** @brief	A bullet debug drawer interface. */
class BulletDebugDrawer : public btIDebugDraw
{
public:

	/**
	 @brief	Draw line.
	
	 @param	from	  	Point A.
	 @param	to		  	Point B.
	 @param	fromColour	from colour.
	 @param	toColour  	to colour.
	 */
	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColour, const btVector3& toColour);

	/**
	@brief	Draw line.

	@param	from	  	Point A.
	@param	to		  	Point B.
	@param	colour		The Colour.
	*/
	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& colour);

	/**
	 @brief	Draw contact point.
	
	 @param	pointOnB 	The point on b.
	 @param	normalOnB	The normal on b.
	 @param	distance 	The distance.
	 @param	lifeTime 	The life time.
	 @param	colour   	The colour.
	 */
	virtual void drawContactPoint(const btVector3 &pointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &colour);

	/**
	 @brief	Reports error or warning.
	
	 @param	warningString	The warning string.
	 */
	virtual void reportErrorWarning(const char* warningString);

	/**
	 @brief	Draw 3D text. (NOT IMPLEMENTED YET)
	
	 @param	location  	The location.
	 @param	textString	The text string.
	 */
	virtual void draw3dText(const btVector3 &location, const char *textString);

	/**
	 @brief	Sets Bullet debug mode.
	
	 @param	inDebugMode	The debug mode.
	 */
	virtual void setDebugMode(int inDebugMode);

	/**
	 @brief	Gets Bullet debug mode.
	
	 @return	The debug mode.
	 */
	virtual int getDebugMode() const;

private:

	/** @brief	The debug mode. */
	int debugMode;
};