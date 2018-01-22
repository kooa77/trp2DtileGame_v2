#pragma once

#include <string>

class Component;

struct sMessageParam
{
	Component* sender;
	Component* receiver;
	std::wstring message;
	int attackPoint;
	int recoveryHP;
};
