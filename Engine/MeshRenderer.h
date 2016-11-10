#pragma once

#include "Component.h"

#include "Material.h"

class MeshRenderer : public Component
{
public:

	virtual ~MeshRenderer();

	
	std::weak_ptr<Material> getMaterial();

	std::vector<std::weak_ptr<Material>> getMaterials();

	void setMaterial(std::weak_ptr<Material> material);

	void setMaterials(std::vector<std::weak_ptr<Material>> newMaterials);


	virtual void onRender();

private:

	std::vector<std::weak_ptr<Material>> materials;
};