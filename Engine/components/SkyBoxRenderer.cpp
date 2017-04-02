#include "SkyBoxRenderer.h"

#include "Transform.h"
#include "../Graphics.h"
#include "MeshComponent.h"
#include "../GameModel.h"
#include "Camera.h"
#include "../Platform.h"

SkyBoxRenderer::~SkyBoxRenderer()
{
}

void SkyBoxRenderer::setMaterial(std::weak_ptr<Material> material)
{
	this->materialRef = material;
}

void SkyBoxRenderer::onPreRender()
{
	if (materialRef.expired())
	{
		Log::logW("Material weak ptr inside SkyBoxRenderer has expired");
		return;
	}
	std::shared_ptr<Material> material = materialRef.lock();


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
	if (GameVariables::data->currentCamera.expired())
	{
		Log::logE("No camera available in MeshRenderer::render");
		return;
	}

	std::shared_ptr<Camera> camera = GameVariables::data->currentCamera.lock();

	glm::mat4 viewMat = camera->getTransformMat();
	viewMat = glm::mat4(glm::mat3(viewMat));
	
	glm::mat4 modelMat = transform->getTransformMat();


	//Disable Depth so the Skybox is always behind the scene
	glDepthMask(GL_FALSE);

	for (unsigned int curMesh = 0; curMesh < mesh->getMeshCount(); curMesh++)
	{
		if (!Platform::isDummyRenderer())
		{
			std::shared_ptr<Shader> shader = material->getShader().lock();
			
			material->useProgram();
			shader->setUniform("modelMat", modelMat);
			shader->setUniform("viewMat", viewMat);
			shader->setUniform("projMat", camera->getProjMat());

			Graphics::renderMesh(mesh, curMesh, material);
		}
	}

	glDepthMask(GL_TRUE);
}
