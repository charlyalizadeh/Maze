#pragma once
#include "Engine.h"

int main() 
{
	Engine* test = new Engine(30,30,3);
	test->Start();
	return 0;
}