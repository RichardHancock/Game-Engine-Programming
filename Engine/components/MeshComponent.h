#pragma once

#include "Component.h"

#include "../GameModel.h"

/** @brief	A component that stores a reference to a mesh. */
class MeshComponent : public Component
{
public:

	/** @brief	Destructor. */
	virtual ~MeshComponent();

	/**
	 @brief	Sets the mesh.
	
	 @param	newMesh	The new mesh.
	 */
	void setMesh(std::weak_ptr<GameModel> newMesh);

	/**
	 @brief	Gets the mesh.
	
	 @return	The mesh.
	 */
	std::weak_ptr<GameModel> getMesh();

private:
	/** @brief	The referenced mesh. */
	std::weak_ptr<GameModel> mesh;
};