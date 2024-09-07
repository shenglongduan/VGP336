#pragma once

namespace SpringEngine
{
	class AppState
	{
	public:
		virtual ~AppState() = default;
		virtual void Initialize() {}
		virtual void Terminate() {}
		virtual void Update(const float deltaTime) {}
		virtual void Render() {}
		virtual void DebugUI() {}
	};
}