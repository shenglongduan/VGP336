#include "Precompiled.h"
#include "GameWorld.h"
#include "GameObjectFactory.h"

#include "CameraService.h"
#include "RenderService.h"
#include "UpdateService.h"
#include "PhysicsService.h"
#include "UIRenderService.h"

#include "TransformComponent.h"
#include "CameraComponent.h"
#include "UIButtonComponent.h"
#include "UISpriteComponent.h"
#include "UITextComponent.h"

#include "SaveUtil.h"

using namespace SpringEngine;

namespace 
{
	CustomService TryService;
}
void GameWorld::SetCustomService(CustomService customService)
{
	TryService = customService;
}

void GameWorld::Initialize()
{
	ASSERT(!mInitialized, "GameObject: is already initialized");

	for (auto& service : mServices)
	{
		service->Initialize();
	}

	mInitialized = true;
}

void SpringEngine::GameWorld::Initialize(uint32_t capacity)
{
	ASSERT(!mInitialized, "GameObject: is already initialized");

	for (auto& service : mServices)
	{
		service->Initialize();
	}
	mGameObjectSlots.resize(capacity);
	mFreeSlot.resize(capacity);
	std::iota(mFreeSlot.begin(), mFreeSlot.end(), 0);

	mInitialized = true;
}

void GameWorld::Terminate()
{
	if (!mInitialized)
	{
		return;
	}
	for (Slot& slot : mGameObjectSlots)
	{
		if (slot.gameObject != nullptr)
		{
			slot.gameObject->Terminate();
			slot.gameObject.reset();
		}
	}

	for (auto& service : mServices)
	{
		service->Terminate();
		service.reset();
	}
	mServices.clear();
	mInitialized = false;
}

void GameWorld::Update(float deltaTime)
{
	for (auto& service : mServices)
	{
		service->Update(deltaTime);
	}

	ProcessDestoryList();
}

void GameWorld::Render()
{
	for (auto& service : mServices)
	{
		service->Render();
	}
}

void GameWorld::DebugUI()
{
	for (Slot& slot : mGameObjectSlots)
	{
		if (slot.gameObject != nullptr)
		{
			slot.gameObject->DebugUI();
		}
	}
	for (auto& service : mServices)
	{
		service->DebugUI();
	}

	if (ImGui::Button("Save"))
	{
		SaveLevel();
	}
}

void GameWorld::LoadLevel(const std::filesystem::path& levelFile)
{
	mLevelFileName = levelFile;
	FILE* file = nullptr;
	auto err = fopen_s(&file, levelFile.u8string().c_str(), "r");
	ASSERT(err == 0 && file != nullptr, "GameWorld: faild to load level %s", levelFile.u8string().c_str());

	char readBuffer[65536];
	rapidjson::FileReadStream readStream(file, readBuffer, sizeof(readBuffer));
	fclose(file);

	rapidjson::Document doc;
	doc.ParseStream(readStream);

	auto services = doc["Services"].GetObj();
	for (auto& service : services)
	{
		std::string serviceName = service.name.GetString();
		Service* newService = TryService(serviceName,*this);
		if (newService != nullptr)
		{
			LOG(" Custom Service %s was successfully added to the world", serviceName.c_str());
		}
		else if (serviceName == "CameraService")
		{
			newService = AddService<CameraService>();
		}
		else if (serviceName == "RenderService")
		{
			newService = AddService<RenderService>();
		}
		else if (serviceName == "UpdateService")
		{
			newService = AddService<UpdateService>();
		}
		else if (serviceName == "PhysicsService")
		{
			newService = AddService<PhysicsService>();
		}
		else if (serviceName == "UIRenderService")
		{
			newService = AddService<UIRenderService>();
		}
		else
		{
			ASSERT(false, "GameObject: service %s is not defined", serviceName.c_str());
		}
		ASSERT(newService != nullptr, "GameObject: service %s was not created properly", serviceName.c_str());
		newService->Deserialize(service.value);
	}
	uint32_t capacity = static_cast<uint32_t>(doc["Capacity"].GetInt());
	Initialize(capacity);

	auto gameobjects = doc["GameObjects"].GetObj();
	for (auto& gameObject : gameobjects)
	{
		std::string name = gameObject.name.GetString();
		const char* templateFile = gameObject.value["Template"].GetString();
		GameObject* obj = CreateGameObject(templateFile, name);
		if (obj != nullptr)
		{
			if (gameObject.value.HasMember("TransformComponent"))
			{
				TransformComponent* transformComponent = obj->GetComponent<TransformComponent>();
				if (transformComponent != nullptr)
				{
					transformComponent->Deserialize(gameObject.value["TransformComponent"].GetObj());
				}
			}
			if (gameObject.value.HasMember("CameraComponent"))
			{
				CameraComponent* cameraComponent = obj->GetComponent<CameraComponent>();
				if (cameraComponent != nullptr)
				{
					cameraComponent->Deserialize(gameObject.value["CameraComponent"].GetObj());
				}
			}if (gameObject.value.HasMember("UISpriteComponent"))
			{
				UISpriteComponent* uispritecomponet = obj->GetComponent<UISpriteComponent>();
				if (uispritecomponet != nullptr)
				{
					uispritecomponet->Deserialize(gameObject.value["UISpriteComponent"].GetObj());
				}
			}
			if (gameObject.value.HasMember("UIButtonComponent"))
			{
				UIButtonComponent* uibuttoncomponent = obj->GetComponent<UIButtonComponent>();
				if (uibuttoncomponent != nullptr)
				{
					uibuttoncomponent->Deserialize(gameObject.value["UIButtonComponent"].GetObj());
				}
			}
			if (gameObject.value.HasMember("UITextComponent"))
			{
				UITextComponent* uiTextComponent = obj->GetComponent<UITextComponent>();
				if (uiTextComponent != nullptr)
				{
					uiTextComponent->Deserialize(gameObject.value["UITextComponent"].GetObj());
				}
			}
			obj->Initialize();
		}
	}
}
void GameWorld::SaveLevel(std::filesystem::path saveFile)
{
	if (saveFile.empty())
	{
		saveFile = mLevelFileName;
	}

	rapidjson::Document doc;
	// capacity (gloal setup)
	// services
	// game objects
	doc.SetObject();
	doc.AddMember("Capacity", static_cast<int>(mGameObjectSlots.size()), doc.GetAllocator());

	rapidjson::Value services(rapidjson::kObjectType);
	for (auto& service : mServices)
	{
		service->Serialize(doc, services);
	}
	doc.AddMember("Services", services, doc.GetAllocator());

	rapidjson::Value gameObjects(rapidjson::kObjectType);
	for (auto& slot : mGameObjectSlots)
	{
		if (slot.gameObject != nullptr)
		{
			rapidjson::Value gameObjectData(rapidjson::kObjectType);
			SaveUtil::SaveString("Template", slot.gameObject->mTemplateFile.c_str(), doc, gameObjectData);
			TransformComponent* transformComponent = slot.gameObject->GetComponent<TransformComponent>();
			if (transformComponent != nullptr)
			{
				transformComponent->Serialize(doc, gameObjectData);
			}
			CameraComponent* cameraComponent = slot.gameObject->GetComponent<CameraComponent>();
			if (cameraComponent != nullptr)
			{
				cameraComponent->Serialize(doc, gameObjectData);
			}
			UISpriteComponent* uiSpriteComponent = slot.gameObject->GetComponent<UISpriteComponent>();
			if (uiSpriteComponent != nullptr)
			{
				uiSpriteComponent->Serialize(doc, gameObjectData);
			}
			UIButtonComponent* uiButtonComponent = slot.gameObject->GetComponent<UIButtonComponent>();
			if (uiButtonComponent != nullptr)
			{
				uiButtonComponent->Serialize(doc, gameObjectData);
			}
			UITextComponent* uiTextComponent = slot.gameObject->GetComponent<UITextComponent>();
			if (uiTextComponent != nullptr)
			{
				uiTextComponent->Serialize(doc, gameObjectData);
			}
			rapidjson::GenericStringRef<char> goName(slot.gameObject->GetName().c_str());
			gameObjects.AddMember(goName, gameObjectData, doc.GetAllocator());
		}
	}
	doc.AddMember("GameObjects", gameObjects, doc.GetAllocator());

	FILE* file = nullptr;
	auto err = fopen_s(&file, saveFile.u8string().c_str(), "w");
	ASSERT(err == 0 && file != nullptr, "GameWorld: filed to open save level %s", saveFile.u8string().c_str());

	char writeBuffer[65536];
	rapidjson::FileWriteStream writeStream(file, writeBuffer, sizeof(writeBuffer));
	rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(writeStream);
	doc.Accept(writer);
	fclose(file);
}

GameObject* SpringEngine::GameWorld::CreateGameObject(const std::filesystem::path& templateFile, const std::string& name)
{
	ASSERT(mInitialized, "GameWorld: is not initilized");

	if (mFreeSlot.empty())
	{
		ASSERT(false, "GameWorld: no free slots available");
		return nullptr;
	}

	const uint32_t freeSlot = mFreeSlot.back();
	mFreeSlot.pop_back();

	Slot& slot = mGameObjectSlots[freeSlot];
	std::unique_ptr<GameObject>& newObject = slot.gameObject;
	newObject = std::make_unique<GameObject>();
	GameObjectFactory::Make(templateFile, *newObject);

	std::string objName = name;
	newObject->SetName(objName);
	newObject->mWorld = this;
	newObject->mTemplateFile = templateFile.u8string().c_str();
	newObject->mHandle.mIndex = freeSlot;
	newObject->mHandle.mGeneration = slot.generation;

	return newObject.get();
}

GameObject* SpringEngine::GameWorld::GetGameObject(const std::string& name)
{
	GameObject* gameObject = nullptr;
	for (Slot& slot : mGameObjectSlots)
	{
		if (slot.gameObject != nullptr && slot.gameObject->GetName() == name)
		{
			gameObject = slot.gameObject.get();
			break;
		}
	}
	return gameObject;
}

GameObject* GameWorld::GetGameObject(const GameObjectHandle& handle)
{
	if (!IsValid(handle))
	{
		return nullptr;
	}
	return mGameObjectSlots[handle.mIndex].gameObject.get();
}

void GameWorld::DestoryGameObject(const GameObjectHandle& handle)
{
	if (!IsValid(handle))
	{
		return;
	}
	Slot& slot = mGameObjectSlots[handle.mIndex];
	slot.generation++;
	mToBeDestroyed.push_back(handle.mIndex);
}

bool GameWorld::IsValid(const GameObjectHandle& handle)
{
	if (handle.mIndex<0 || handle.mIndex>=mGameObjectSlots.size())
	{
		return false;
	}
	if (mGameObjectSlots[handle.mIndex].generation != handle.mGeneration)
	{

	return false;
	}
	return true;
}

void GameWorld::ProcessDestoryList()
{
	for (uint32_t index : mToBeDestroyed)
	{
		Slot& slot = mGameObjectSlots[index];

		GameObject* gameObject = slot.gameObject.get();
		ASSERT(!IsValid(gameObject->GetHandle()), "GameWorld : object is still alive");

		gameObject->Terminate();
		slot.gameObject.reset();
		mFreeSlot.push_back(index);
	}
	mToBeDestroyed.clear();
}

