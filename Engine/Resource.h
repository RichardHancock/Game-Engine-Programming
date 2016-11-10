#pragma once

class Resource
{
public:
	Resource();

	bool keepLoaded();
	void keepLoaded(bool active);

protected:
	unsigned int instances;
	bool keepResourceLoaded;
};