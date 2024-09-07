#include <SpringEngine/Inc/SpringEngine.h>

#include "GameState.h"

using namespace SpringEngine;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	AppConfig config;
	config.appName = L"Hello Shapes";

	App& myApp = MainApp();
	myApp.AddState<Triangle>("Triangle");
	myApp.AddState<Square>("Square");
	myApp.Run(config);

	return(0);
}