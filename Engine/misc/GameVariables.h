#pragma once

#include <memory>
#include <unordered_map>

class Camera;
class GameObject;

struct GameVariableData
{
	std::weak_ptr<Camera> currentCamera;
	std::weak_ptr<GameObject> currentLight;
	std::unordered_map<std::string, std::shared_ptr<GameObject>> gameObjs;
};

/** @brief	Contains any game wide data that need to be accessable semi-globally. */
class GameVariables
{
public:
	static std::shared_ptr<GameVariableData> data;
};