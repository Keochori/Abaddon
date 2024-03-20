#pragma once
#include "Scene/Entity.h"

class Script
{
public:
	Script() = default;
	virtual ~Script() {};

	template<typename T>
	T& GetComponent()
	{
		return myEntity.GetComponent<T>();
	}

protected:
	virtual void Start() {};
	virtual void OnDestroy() {};
	virtual void Update() {};

private:
	Entity myEntity;
	friend class Scene;
};

