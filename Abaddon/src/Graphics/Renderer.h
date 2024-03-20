#pragma once
#include "Bindables/CBuffer.hpp"

struct ModelData;
struct TextureData;
struct Transform;
class Camera;

class Renderer
{
public:
	void Init();
	void Render(ModelData& aModelData, TextureData& aTextureData, Transform& aTransform, std::shared_ptr<Camera> aCamera);

private:
	CBuffer<TransformBuffer> myCBufferTransform;
};

