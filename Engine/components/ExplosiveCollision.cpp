#include "ExplosiveCollision.h"

#include "../Physics.h"
#include "../ResourceManager.h"

ExplosiveCollision::~ExplosiveCollision()
{
}

void ExplosiveCollision::onCollision(std::string objAName, std::string objBName, glm::vec3 contactPoint)
{
	if (objAName.find("Rocket") != std::string::npos || objBName.find("Rocket") != std::string::npos)
	{
		Physics::triggerExplosion(contactPoint, 50.0f, 5.0f);

		ResourceManager::getAudio("335152__apenguin73__explosion-test.wav", false).lock()->play(0, 0);

		getGameObject().lock()->setDeleteFlag();
	}
}
