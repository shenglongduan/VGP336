#include "Precompiled.h"
#include "PhysicsDebugDraw.h"

#include <Graphics/Inc/SimpleDraw.h>

using namespace SpringEngine;
using namespace SpringEngine::Physics;
using namespace SpringEngine::Graphics;

void PhysicsDebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	SimpleDraw::AddLine(ConvertTobtVector3(from), ConvertTobtVector3(to), ConvertToColor(color));
}

void PhysicsDebugDraw::drawContactPoint(const btVector3& pointOn, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
	//NOTHING HERE
}

void PhysicsDebugDraw::reportErrorWarning(const char* warningString)
{
	LOG("[Physics Warning] %s", warningString);
}

void PhysicsDebugDraw::draw3dText(const btVector3& location, const char* textString)
{
	//NOTHING HERE
}

void PhysicsDebugDraw::setDebugMode(int debugMode)
{
	mDebugMode = debugMode;
}

int PhysicsDebugDraw::getDebugMode() const
{
	return mDebugMode;
}
