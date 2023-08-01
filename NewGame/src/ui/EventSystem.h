#pragma once

#include <vector>
#include <string>
#include <iostream>

class IObserver
{
public:

	virtual void OnNotify(const std::string& eventName, const void* eventData) = 0;
};

class EventSystem
{
private:

	std::vector<IObserver*> observers;

public:

	void Subscribe(IObserver* observer);
	void Unsubscribe(IObserver* observer);
	void Notify(const std::string& eventName, const void* eventData);
};

class GameObject : public IObserver
{
public:

	void OnNotify(const std::string& eventName, const void* eventData) override
	{
		if (eventName == "EnemyKilled")
		{
			std::cout << "GameObject received notification: " << *((int*)eventData) << " EnemyKilled" << std::endl;
		}
	}
};