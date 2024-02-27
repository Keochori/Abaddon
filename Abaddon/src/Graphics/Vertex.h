#pragma once
#include "Math/vector3.hpp"

struct Vertex
{
	Vertex() {}
	Vertex(float aX, float aY, float aZ) { position.x = aX; position.y = aY; position.z = aZ; }
	Vertex(math::vector3<float>& aPosition) { position = aPosition; }

	math::vector3<float> position;
};