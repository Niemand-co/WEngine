#pragma once
#include "Scene/Public/GameObject.h"

enum class Direction
{
	FORWARD = 0,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

enum class RotateDirection
{
	Pitch = 0,
	Yaw,
	Roll
};

class GameObject;

class Component : public Object
{
	ENABLE_REFLECTION

	friend class GameObject;

public:
	
	enum { type = 0 };

	virtual ~Component() = default;

	//virtual void Tick(const WEngine::TimeStep& timeStep) = 0;

	inline GameObject* GetOwner() { return m_pGameObject; } ;

	virtual void ShowInInspector() = 0;

	void MarkDirty() { m_bMarkedDirty = true; }

protected:

	Component(GameObject *pGameObject)
		: m_pGameObject(pGameObject), m_bMarkedDirty(true)
	{
	};

protected:

	GameObject* m_pGameObject;

	uint8 m_bMarkedDirty : 1;

};

//namespace WEngine
//{
//	namespace SRefl
//	{
//		template<>
//		struct TypeInfo<Component> : public TypeInfoBase<Component, Object>
//		{
//			static constexpr WString name = "Component";
//
//			static constexpr FieldList lis = 
//			{
//				Field{"m_pGameObject", &Component::m_pGameObject, AttrList{}},
//				Field{"m_type", &Component::m_type, AttrList{}},
//				Field{"GetType", &Component::GetType, AttrList{}},
//				Field{"IsType", &Component::IsType, AttrList{}},
//			};
//		};
//	}
//}