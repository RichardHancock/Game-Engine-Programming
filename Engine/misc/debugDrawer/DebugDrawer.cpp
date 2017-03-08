#include "DebugDrawer.h"

#include <GL/glew.h>

#include "../GameVariables.h"
#include "../../components/Camera.h"
#include "../../ResourceManager.h"
#include "../../Platform.h"

std::shared_ptr<DebugPrimitives> DebugDrawer::lines;
std::shared_ptr<DebugPrimitives> DebugDrawer::points;
std::shared_ptr<Shader> DebugDrawer::shader;
bool DebugDrawer::zeroDepthWireFrame = false;

void DebugDrawer::init(std::string vShaderFilename, std::string fShaderFilename)
{
	shader = std::make_shared<Shader>(
		ResourceManager::getResourceDirPath() + ResourceManager::shaderDir + vShaderFilename,
		ResourceManager::getResourceDirPath() + ResourceManager::shaderDir + fShaderFilename
		);

	lines = std::make_shared<DebugPrimitives>(GL_LINES);
	points = std::make_shared<DebugPrimitives>(GL_POINTS);

	Log::logI("Debug Drawer SubSystem Initialized");
}

void DebugDrawer::cleanup()
{
	shader.reset();

	lines.reset();
	points.reset();
}

void DebugDrawer::preRender()
{
	lines->populateVBO();
	points->populateVBO();
}

void DebugDrawer::render()
{
	//Get Camera Matrices
	if (GameVariables::data->currentCamera.expired())
	{
		Log::logE("No camera available in MeshRenderer::render");
		return;
	}

	std::shared_ptr<Camera> camera = GameVariables::data->currentCamera.lock();

	glm::mat4 viewMat = glm::inverse(camera->getTransformMat());
	glm::mat4 projMat = camera->getProjMat();

	if (!Platform::isDummyRenderer())
	{
		if (zeroDepthWireFrame)
			glDisable(GL_DEPTH_TEST);

		glUseProgram(shader->getProgram());

		//Set Uniforms
		shader->setUniform("viewMat", viewMat);
		shader->setUniform("projMat", projMat);
	
		lines->render();
		points->render();

		glUseProgram(0);

		if (zeroDepthWireFrame)
			glEnable(GL_DEPTH_TEST);
	}
}

void DebugDrawer::postRender()
{
	lines->clearData();
	points->clearData();
}

void DebugDrawer::addLine(glm::vec3 pA, glm::vec3 pB, glm::vec3 colour)
{
	lines->addData(pA);
	lines->addData(colour);
	lines->addData(pB);
	lines->addData(colour);
}

void DebugDrawer::addLine(glm::vec3 pA, glm::vec3 pB, glm::vec3 pAColour, glm::vec3 pBColour)
{
	lines->addData(pA);
	lines->addData(pAColour);
	lines->addData(pB);
	lines->addData(pBColour);
}

void DebugDrawer::addLine(Line line)
{
	lines->addData(line.pA);
	lines->addData(line.colour);
	lines->addData(line.pB);
	lines->addData(line.colour);
}

void DebugDrawer::addContactPoint(glm::vec3 point, glm::vec3 normal, glm::vec3 colour)
{
	glm::vec3 to = (point + normal);
	
	points->addData(point);
	points->addData(colour);

	//Normal
	lines->addData(point);
	lines->addData(colour);
	lines->addData(to);
	lines->addData(colour);
}

void DebugDrawer::addContactPoint(ContactPoint point)
{
	glm::vec3 to = (point.point + point.normal) * 3.0f;

	points->addData(point.point);
	points->addData(point.colour);

	//Normal
	lines->addData(point.point);
	lines->addData(point.colour);
	lines->addData(to);
	lines->addData(point.colour);
}

void DebugDrawer::setPointSize(float pointSize)
{
	if (!Platform::isDummyRenderer())
	{
		glPointSize(pointSize);
	}
}

void DebugDrawer::setLineSize(float lineSize)
{
	if (!Platform::isDummyRenderer())
	{
		glLineWidth(lineSize);
	}
}

void DebugDrawer::toggleZeroDepthWireFrame()
{
	zeroDepthWireFrame = !zeroDepthWireFrame;
}
