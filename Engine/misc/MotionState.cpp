#include "MotionState.h"

btTransform MotionState::getGameTransform()
{
	return transform;
}

void MotionState::getWorldTransform(btTransform & worldTrans)
{
	worldTrans = transform;
}

void MotionState::setWorldTransform(const btTransform & worldTrans)
{
	transform = worldTrans;
}
