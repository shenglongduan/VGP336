#include <SpringEngine/Inc/SpringEngine.h>
#include "GameState.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
    SpringEngine::App& app = SpringEngine::MainApp();
    app.AddState<GameState>("GameState");

    SpringEngine::AppConfig config;
    config.appName = L"Hello TransparencyEffect";
    config.winWidth = 1280;
    config.winHeight = 720;

    app.Run(config);
    return 0;
}