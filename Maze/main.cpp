#pragma once
#include "Engine.h"

int main() 
{
	Engine* test = new Engine(20,20,3);
	test->Start();
	return 0;
}