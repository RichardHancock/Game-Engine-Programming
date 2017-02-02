#include "CollisionShape.h"

#include <glm/glm.hpp>

#include "MeshComponent.h"

bool CollisionShape::generateStaticMeshShape()
{
	std::weak_ptr<MeshComponent> meshWrapRef = getGameObject().lock()->getComponent<MeshComponent>("MeshComponent");

	if (meshWrapRef.expired())
	{
		Log::logW("Cannot generate a Mesh Collision shape with no MeshComponent attached to game object.");
		return false;
	}

	std::shared_ptr<MeshComponent> meshWrap = meshWrapRef.lock();

	//Model Check
	std::weak_ptr<GameModel> modelRef = meshWrap->getMesh();

	if (modelRef.expired())
	{
		Log::logW("Cannot generate a Mesh Collision shape with no Mesh attached to game object's Mesh Component.");
		return false;
	}



	std::shared_ptr<GameModel> model = modelRef.lock();

	std::vector<glm::vec3> vertices = model->getVertices();
	if (vertices.empty())
	{
		Log::logE("Cannot generate a Mesh Collision shape as vertex array is empty");
		return false;
	}

	std::vector<unsigned int> indices = model->getIndices();
	if (indices.empty())
	{
		Log::logE("Cannot generate a Mesh Collision shape as index array is empty");
		return false;
	}

	/*
	int triangleStride = sizeof(unsigned int);
	int vertexStride = sizeof(float);

	std::vector<float> verticesConverted;
	verticesConverted.reserve(vertices.size() * 3);

	for (auto vertex : vertices)
	{
		verticesConverted.push_back(vertex.x);
		verticesConverted.push_back(vertex.y);
		verticesConverted.push_back(vertex.z);
	}

	auto shapeRaw = new btTriangleIndexVertexArray(
		indices.size() / 3,
		(int*)&indices[0],
		triangleStride,
		verticesConverted.size(),
		&verticesConverted[0],
		vertexStride
		);

	shape = std::make_shared<btBvhTriangleMeshShape>(shapeRaw, true);
	*/
	
	btConvexHullShape* rawShape = new btConvexHullShape();
	for (auto vertex : vertices)
	{
		rawShape->addPoint(btVector3(vertex.x, vertex.y, vertex.z));
	}

	shape = rawShape;

	return true;
}

btCollisionShape* CollisionShape::getShape()
{
	return shape;
}
