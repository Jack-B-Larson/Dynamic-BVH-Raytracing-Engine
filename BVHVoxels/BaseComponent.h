#pragma once
#include <typeindex>

class GameObject;

class BaseComponent
{
public:
	virtual ~BaseComponent() = default;

	virtual std::type_index GetType() = 0;
	void SetOwner(GameObject* newOwner);
	GameObject* GetOwner() const;

	virtual void Initialize();
	virtual void PreUpdate();
	virtual void Update();
	virtual void PostUpdate();
	virtual void Render();

	bool GetEnabled() const;

	void Enable();
	void Disable();

private:
	bool enabled = false;
	GameObject* owner = nullptr;
};

