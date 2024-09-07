#pragma once

//framework headers
#include <Math/Inc/SpMath.h>
#include <Core/Inc/Core.h>
#include <Graphics/Inc/Transform.h>
#include <Graphics/Inc/Colors.h>

//bullet files
#include <Bullet/btBulletCollisionCommon.h>
#include <Bullet/btBulletDynamicsCommon.h>
#include <Bullet/BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include <Bullet/BulletSoftBody/btSoftBodyHelpers.h>
#include <Bullet/BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>
#include <Bullet/BulletSoftBody/btSoftBodySolvers.h>


//functions
template<class T>
inline void SafeDelete(T*& ptr)
{
	if (ptr != nullptr)
	{
		delete ptr;
		ptr = nullptr;
	}
}

inline btVector3 ConvertTobtVector3(const SpringEngine::Math::Vector3& v)
{
	return btVector3(v.x, v.y, v.z);
}

inline SpringEngine::Math::Vector3 ConvertTobtVector3(const btVector3& v)
{
	return { 
		static_cast<float>(v.getX()),
		static_cast<float>(v.getY()),
		static_cast<float>(v.getZ())
	};
}

inline SpringEngine::Color ConvertToColor(const btVector3& v)
{
	return {
		static_cast<float>(v.getX()),
		static_cast<float>(v.getY()),
		static_cast<float>(v.getZ()),
		1.0f
	};
}
inline btQuaternion ConvertTobtQuaternion(const SpringEngine::Math::Quaternion& q)
{
	return btQuaternion(q.x, q.y, q.z,q.w);
}
inline SpringEngine::Math::Quaternion ConvertTobtQuaternion(const btQuaternion& q)
{
	return {
		static_cast<float>(q.getX()),
		static_cast<float>(q.getY()),
		static_cast<float>(q.getZ()),
		static_cast<float>(q.getW()),
	};
}

inline btTransform ConvertTobyTransform(const SpringEngine::Graphics::Transform& trans)
{
	return btTransform(ConvertTobtQuaternion(trans.rotation), ConvertTobtVector3(trans.position));
}