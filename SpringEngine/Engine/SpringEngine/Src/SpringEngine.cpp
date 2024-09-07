#include "Precompiled.h"
#include "SpringEngine.h"

SpringEngine::App& SpringEngine::MainApp()
{
	static App sApp;
	return sApp;
}
