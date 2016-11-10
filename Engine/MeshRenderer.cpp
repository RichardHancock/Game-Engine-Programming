#include "MeshRenderer.h"

#include "MeshComponent.h"
#include "GameModel.h"
#include "Transform.h"
#include "Graphics.h"

MeshRenderer::~MeshRenderer()
{
}

std::weak_ptr<Material> MeshRenderer::getMaterial()
{
	if (materials.empty()) {
		return std::weak_ptr<Material>();
	}
	else
	{
		return materials[0];
	}
}

std::vector<std::weak_ptr<Material>> MeshRenderer::getMaterials()
{
	return materials;
}

void MeshRenderer::setMaterial(std::weak_ptr<Material> material)
{
	if (materials.empty())
	{
		materials.push_back(material);
	}
	else
	{
		materials[0] = material;
	}
}

void MeshRenderer::setMaterials(std::vector<std::weak_ptr<Material>> newMaterials)
{
	materials = newMaterials;
}

void MeshRenderer::render()
{
	std::shared_ptr<GameObject> gameObj = getGameObject().lock();

	//Mesh
	std::weak_ptr<MeshComponent> meshHolder = gameObj->getComponent<MeshComponent>("MeshComponent");

	if (meshHolder.expired()) 
	{
		Log::logE("Mesh Component is expired at MeshRenderer::render");
		return;
	}

	std::weak_ptr<GameModel> meshRef = meshHolder.lock()->getMesh();

	if (meshRef.expired())
	{
		Log::logE("Mesh is expired at MeshRenderer::render");
		return;
	}

	std::shared_ptr<GameModel> mesh = meshRef.lock();


	//Transform
	std::weak_ptr<Transform> transRef = gameObj->getComponent<Transform>("Transform");

	if (transRef.expired())
	{
		Log::logE("Transform is expired at MeshRenderer::render");
		return;
	}

	std::shared_ptr<Transform> transform = transRef.lock();


	//Calculate Matrices
	if (ResourceManager::engineState->currentCamera.expired())
	{
		Log::logE("No camera availabe in MeshRenderer::render");
		return;
	}

	std::shared_ptr<Camera> camera = ResourceManager::engineState->currentCamera.lock();

	glm::mat4 viewMat = camera->getTransformMat();
	glm::mat4 modelMat = transform->getTransformMat();


	///TODO Implement sub meshes
	
	//Material
	std::weak_ptr<Material> materialRef = getMaterial();

	if (materialRef.expired())
	{
		Log::logE("No material provided to mesh renderer");
		return;
	}

	std::shared_ptr<Material> material = materialRef.lock();

	std::shared_ptr<Shader> shader = material->getShader().lock();

	shader->setUniform("modelMat", modelMat);
	shader->setUniform("viewMat", viewMat);
	shader->setUniform("projMat", camera->getProjMat());

	material->useProgram();
	Graphics::renderMesh(mesh, material);
	
}
