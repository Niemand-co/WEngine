#pragma once
#include "Scene/Public/Object.h"

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
	enum class ComponentType
	{
		Transformer = 0,
		Camera,
		MeshFilter,
		Material,
	};

public:

	typedef Component type;

	virtual ~Component() = default;

	virtual ComponentType GetType() { return m_type; }

	inline GameObject* GetGameObject() { return m_pGameObject; } ;

	virtual void ShowInInspector() = 0;

	virtual bool IsType(ComponentType type) 
	{
		return type == m_type; 
	}

protected:

	Component(GameObject *pGameObject)
		: m_pGameObject(pGameObject)
	{
	};

protected:

	GameObject* m_pGameObject;

	ComponentType m_type;

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