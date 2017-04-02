#pragma once

#include "Component.h"

#include "../Material.h"

class SkyBoxRenderer : public Component
{
public:
	virtual ~SkyBoxRenderer();

	void setMaterial(std::weak_ptr<Material> material);

	virtual void onPreRender();

protected:
	
	std::weak_ptr<Material> materialRef;

};