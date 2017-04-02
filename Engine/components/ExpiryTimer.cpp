#include "ExpiryTimer.h"

ExpiryTimer::ExpiryTimer()
{
	timer = nullptr;
}

ExpiryTimer::~ExpiryTimer()
{
	if (timer != nullptr)
		delete timer;
}

void ExpiryTimer::onUpdate()
{
	if (timer != nullptr)
	{
		timer->update();

		if (timer->hasTimerFinished())
		{
			getGameObject().lock()->setDeleteFlag();
			return;
		}
	}
}

void ExpiryTimer::startExpiryTimer(float length)
{
	timer = new Utility::SimpleTimer(length);
}
