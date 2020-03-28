#pragma once
#include "Engine.h"

int main() 
{
	Engine* test = new Engine(15,15,6,FG_BLACK,FG_MAGENTA,FG_WHITE);
	test->Start();
	return 0;
}