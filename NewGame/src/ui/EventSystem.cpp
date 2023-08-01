#include "EventSystem.h"

void EventSystem::Subscribe(IObserver* observer)
{
	observers.push_back(observer);
}

void EventSystem::Unsubscribe(IObserver* observer)
{
	auto it = std::find(observers.begin(), observers.end(), observer);
	if (it != observers.end())
	{
		observers.erase(it);
	}
}

void EventSystem::Notify(const std::string& eventName, const void* eventData)
{
	for (IObserver* observer : observers)
	{
		observer->OnNotify(eventName, eventData);
	}
}