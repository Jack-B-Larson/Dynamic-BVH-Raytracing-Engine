#pragma once
#include <typeindex>
#include "BaseComponent.h"
template<class componentType>
class Component : public BaseComponent
{
public:
	static constexpr std::type_index GetComponentType()
	{
		return std::type_index(typeid(componentType));
	}
	virtual std::type_index GetType()
	{
		return GetComponentType();
	}
};

