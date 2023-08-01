#pragma once

class Entity;

class Component
{
public:

	Entity* entity;

	virtual ~Component() {};
};