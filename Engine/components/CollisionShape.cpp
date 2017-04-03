#include "CollisionShape.h"

#include <glm/glm.hpp>

#include "MeshComponent.h"
#include "../misc/Utility.h"

CollisionShape::~CollisionShape()
{
	delete shape;
}

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

	bool indicesAvailable = true;
	std::vector<unsigned int> indices = model->getIndices();
	if (indices.empty())
		indicesAvailable = false;

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
	
	btTriangleMesh* triangles = new btTriangleMesh(); //TODO Possible mem leak
	
	if (indicesAvailable)
	{
		for (unsigned int index = 0; index < indices.size(); index += 3)
		{
			triangles->addTriangle(
				Utility::glmToBulletVec3(vertices[indices[index]]),
				Utility::glmToBulletVec3(vertices[indices[index + 1]]),
				Utility::glmToBulletVec3(vertices[indices[index + 2]])
			);
		}
	}
	else
	{
		for (unsigned int vertexIndex = 0; vertexIndex < indices.size(); vertexIndex += 3)
		{
			triangles->addTriangle(
				Utility::glmToBulletVec3(vertices[vertexIndex]),
				Utility::glmToBulletVec3(vertices[vertexIndex + 1]),
				Utility::glmToBulletVec3(vertices[vertexIndex + 2])
			);
		}
	}


	/*
	btConvexHullShape* rawShape = new btConvexHullShape();
	for (auto vertex : vertices)
	{
		rawShape->addPoint(btVector3(vertex.x, vertex.y, vertex.z));
	}*/

	//btConvexTriangleMeshShape* rawShape = new btConvexTriangleMeshShape(triangles);
	btBvhTriangleMeshShape* rawShape = new btBvhTriangleMeshShape(triangles, false);
	shape = rawShape;

	shape->setMargin(2.0f);
	return true;
}

bool CollisionShape::generateConvexMeshShape()
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

	bool indicesAvailable = true;

	std::vector<unsigned int> indices = model->getIndices();
	if (indices.empty())
		indicesAvailable = false;


	btTriangleMesh* triangles = new btTriangleMesh(); //TODO Possible mem leak

	if (indicesAvailable)
	{
		for (unsigned int index = 0; index < indices.size(); index += 3)
		{
			triangles->addTriangle(
				Utility::glmToBulletVec3(vertices[indices[index]]),
				Utility::glmToBulletVec3(vertices[indices[index + 1]]),
				Utility::glmToBulletVec3(vertices[indices[index + 2]])
			);
		}
	}
	else
	{
		for (unsigned int vertexIndex = 0; vertexIndex < indices.size(); vertexIndex += 3)
		{
			triangles->addTriangle(
				Utility::glmToBulletVec3(vertices[vertexIndex]),
				Utility::glmToBulletVec3(vertices[vertexIndex + 1]),
				Utility::glmToBulletVec3(vertices[vertexIndex + 2])
			);
		}
	}

	btConvexTriangleMeshShape* rawShape = new btConvexTriangleMeshShape(triangles);
	shape = rawShape;

	shape->setMargin(2.0f);
	return true;
}

bool CollisionShape::generateBoxShape()
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

	btBoxShape* boxShape = new btBoxShape(Utility::glmToBulletVec3(model->getAABB().halfSize));

	shape = boxShape;

	return true;
}

btCollisionShape* CollisionShape::getShape()
{
	return shape;
}
