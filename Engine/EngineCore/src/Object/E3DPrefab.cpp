#include "E3DPrefab.h"
#include "../Scene/E3DScene.hpp"
#include "../Scene/E3DSceneLoader.h"

E3DEngine::Prefab::Prefab()
{
	mSceneObjectType = TP_Prefab;
	CreateBehaviour();
}

void E3DEngine::Prefab::SetFilePath(std::string path)
{
	mFilePath = path;
}

std::string E3DEngine::Prefab::GetFilePath()
{
	return mFilePath;
}

void E3DEngine::Prefab::CreateBehaviour()
{
	mBehaviour->SetImage(MonoScriptManager::GetInstance().GetEngineImage());
	NEW_INSTANCE(Prefab);
	setBehaviourDefaultValue();
}

E3DEngine::Prefab * E3DEngine::Prefab::LoadPrefab(std::string path)
{
	return LoadPrefab(path);
}

void E3DEngine::Prefab::Save()
{
	SavePrefab(this);
}