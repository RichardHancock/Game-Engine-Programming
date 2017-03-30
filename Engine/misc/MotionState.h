#pragma once

#include <btBulletCollisionCommon.h>

class MotionState : public btMotionState
{
public:

	btTransform getGameTransform();

	virtual void getWorldTransform(btTransform& worldTrans);

	virtual void setWorldTransform(const btTransform& worldTrans);

protected:

	btTransform transform;
};