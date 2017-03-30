#pragma once

#include "../internal/dependencies/btBulletDynamicsCommon.h"
#include <memory>

#include "../components/Transform.h"

class MotionState : public btMotionState
{
public:

	MotionState(std::weak_ptr<Transform> linkedTransformRef);
	
	virtual void getWorldTransform(btTransform& worldTrans) const;

	virtual void setWorldTransform(const btTransform& worldTrans);

protected:

	std::weak_ptr<Transform> transformRef;
};