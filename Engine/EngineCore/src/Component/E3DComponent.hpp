﻿//********************************** Effective 3D Engine **************************************************//
//******************* Copyright (c) 2017-2-8  PanQingyun. All rights reserved. *************************//
//
// 组件
// 创建：2017-2-8
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef E3DComponent_hpp
#define E3DComponent_hpp

#include "../Source/Interface.h"
#include "../Mono/MonoScriptManager.h"
#include "../Object/E3DObject.h"

void ImplComponent();
namespace E3DEngine
{
	enum FieldTypeEnum
	{
		FT_INT		= 1 << 0,
		FT_FLOAT	= 1 << 1,
		FT_STRING	= 1 << 2,
		FT_BOOLEAN	= 1 << 3,
		FT_OBJECT	= 1 << 4,
		FT_COLOR	= 1 << 5,
		FT_VECTOR3  = 1 << 6,
		FT_VECTOR2	= 1 << 7,

		FT_READONLY = 1 << 31,
	};

#define NOT_RUN_RETURN \
if(!E3DEngine::EngineDelegate::GetInstance().GetIsRun())\
{\
	return;\
}

    class GameObject;
	class CTransform;
	// 组件类， 目的是方便拓展， 可以挂在GameObject上
    class E3D_EXPORT_DLL Component : public Object
    {
    public:
		Component();
		virtual void OnCreate();
		virtual void OnCreateComplete();
		virtual void Awake();
        virtual void Start();
        virtual void Update(float deltaTime);
		virtual void LateUpdate(float deltaTime);
        virtual void Destory();
		virtual void OnEnable();
		virtual void OnDisable();
		virtual void OnCollisionEnter(GameObject* go);
		virtual void SetGameObject(GameObject *go);
		GameObject * GetGameObject();
		virtual void SetPropertyValue(void* value);
		virtual void registProperty();

		virtual std::string GetFieldValueStr(std::string fieldName);
		virtual void SetFieldValue(std::string fieldName, std::string fieldValueStr);
		virtual void SetActive(bool active);
		virtual Object *Get() { return this; }
		virtual void TransformChange() { }
	public:
        GameObject *	mGameObject;
		std::string 	_Tag;
		CTransform *    Transform;
		bool			IsActive;

	public:
		bool			mNotStart;

		map<std::string, void*> m_setMethodMap;
		map<std::string, void*> m_getMethodMap;
		map<std::string, DWORD> m_propertyTypeMap;

	private: 
		std::string getMonoFieldValueStr(std::string fieldName);
		std::string getEngineFieldValueStr(std::string fieldName);
		void setMonoFieldVale(std::string fieldName, std::string fieldValueStr);
		void setEngineFieldValue(std::string fieldName, std::string fieldValueStr);

    };
}

#endif /* CComponent_hpp */
