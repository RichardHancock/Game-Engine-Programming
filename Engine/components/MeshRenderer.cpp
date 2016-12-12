#include "MeshRenderer.h"

#include "MeshComponent.h"
#include "../GameModel.h"
#include "Transform.h"
#include "../Graphics.h"
#include "../misc/GameVariables.h"
#include "Camera.h"
#include "Light.h"

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

void MeshRenderer::onRender()
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
	if (GameVariables::data->currentCamera.expired())
	{
		Log::logE("No camera availabe in MeshRenderer::render");
		return;
	}

	std::shared_ptr<Camera> camera = GameVariables::data->currentCamera.lock();

	glm::mat4 viewMat = glm::inverse(camera->getTransformMat());
	glm::mat4 modelMat = transform->getTransformMat();

	if (materials.size() == 0)
		return;

	//Fetch the light
	std::weak_ptr<GameObject> lightObj = GameVariables::data->currentLight;
	if (lightObj.expired())
	{
		Log::logE("No lights in scene, this can cause unforceen behaviour");
		return;
	}
	std::shared_ptr<Light> light = lightObj.lock()->getComponent<Light>().lock();

	for (unsigned int curMesh = 0; curMesh < mesh->getMeshCount(); curMesh++)
	{
		//Material
		std::weak_ptr<Material> materialRef = materials[mesh->getSubmesh(curMesh).materialIndex];

		if (materialRef.expired())
		{
			Log::logE("No material provided to mesh renderer");
			return;
		}

		std::shared_ptr<Material> material = materialRef.lock();

		std::shared_ptr<Shader> shader = material->getShader().lock();
		material->useProgram();
		shader->setUniform("modelMat", modelMat);
		shader->setUniform("viewMat", viewMat);
		shader->setUniform("projMat", camera->getProjMat());
		shader->setUniform("lightPos", light->getPosition());
		shader->setUniform("viewPos", glm::vec3(viewMat[3][0], viewMat[3][1], viewMat[3][2]));

		shader->setUniform("pointlight.constant", light->getConstant());
		shader->setUniform("pointlight.linear", light->getLinear());
		shader->setUniform("pointlight.quadratic", light->getQuadratic());
		shader->setUniform("pointlight.ambient", light->getAmbient());
		shader->setUniform("pointlight.diffuse", light->getDiffuse());
		shader->setUniform("pointlight.specular", light->getSpecular());
		shader->setUniform("pointlight.position", light->getPosition());
		
		Graphics::renderMesh(mesh, curMesh, material);
	}
}
