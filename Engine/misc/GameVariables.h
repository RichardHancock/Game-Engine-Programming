#pragma once

#include <memory>
#include <unordered_map>

class Camera;
class GameObject;

struct GameVariableData
{
	std::weak_ptr<Camera> currentCamera;
	std::unordered_map<std::string, std::shared_ptr<GameObject>> gameObjs;
};

class GameVariables
{
public:
	static std::shared_ptr<GameVariableData> data;
};