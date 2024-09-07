#include "Precompiled.h"
#include "SoftBody.h"

#include "PhysicsWorld.h"

using namespace SpringEngine;
using namespace SpringEngine::Physics;

SoftBody::~SoftBody()
{
	ASSERT(mSoftBody == nullptr, "SoftBody: must call terminate");
}

void SoftBody::Initialize(Graphics::Mesh& mesh, float mass, const std::vector<int>& fixedNodeIndices)
{
	mMesh = &mesh;
	mSoftBody = PhysicsWorld::Get()->CreateSoftBody(mesh.vertices.size());

	btSoftBody::Material* pm = mSoftBody->appendMaterial();
	pm->m_kAST = 1.0f;
	pm->m_kLST = 1.0f;
	pm->m_kVST = 1.0f;
	mSoftBody->m_cfg.kMT = 0.2f;
	for (size_t i = 0; i < mesh.vertices.size(); ++i)
	{
		mSoftBody->m_nodes[i].m_x = ConvertTobtVector3(mesh.vertices[i].position);
	}

	for (size_t i = 0; i < mesh.indices.size(); i += 6)
	{
		int index0 = mesh.indices[i];
		int index1 = mesh.indices[i + 4];
		int index2 = mesh.indices[i + 5];
		int index3 = mesh.indices[i + 2];

		mSoftBody->appendLink(index0, index1);
		mSoftBody->appendLink(index1, index2);
		mSoftBody->appendLink(index2, index3);
		mSoftBody->appendLink(index3, index1);
	}
	for (auto& fixedNode : fixedNodeIndices)
	{
		mSoftBody->m_nodes[fixedNode].m_im = 0.0f;
	}

	PhysicsWorld::Get()->Register(this);
}

void SoftBody::Terminate()
{
	PhysicsWorld::Get()->Unregister(this);
	SafeDelete(mSoftBody);
}

void SoftBody::Update()
{
	for (size_t i = 0; i < mMesh->vertices.size(); ++i)
	{
		mMesh->vertices[i].position = ConvertTobtVector3(mSoftBody->m_nodes[i].m_x);
	}
}

btSoftBody* SoftBody::GetSoftBody()
{
	return mSoftBody;
}
