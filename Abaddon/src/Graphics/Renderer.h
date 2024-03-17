#pragma once
#include "Scene/Model.h"

class Renderer
{
public:
	Renderer() = default;
	~Renderer() = default;

	void Render(std::vector<std::shared_ptr<Model>> aModelList);

private:

};

