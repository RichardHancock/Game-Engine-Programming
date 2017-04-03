#pragma once

#include "../internal/dependencies/btBulletDynamicsCommon.h"
#include <memory>

#include "../components/Transform.h"

/** @brief	A Bullet motion state. (Keeps my game objects in sync with physics data */
class MotionState : public btMotionState
{
public:

	/**
	 @brief	Constructor.
	
	 @param	linkedTransformRef	The linked transform.
	 */
	MotionState(std::weak_ptr<Transform> linkedTransformRef);

	/**
	 @brief	Gets world transform.
	
	 @param [in,out]	worldTrans	The world transform.
	 */
	virtual void getWorldTransform(btTransform& worldTrans) const;

	/**
	 @brief	Sets world transform.
	
	 @param	worldTrans	The world transform.
	 */
	virtual void setWorldTransform(const btTransform& worldTrans);

protected:

	/** @brief	The transform reference. */
	std::weak_ptr<Transform> transformRef;
};