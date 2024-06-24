#pragma once
#include "Bindables/CBuffer.hpp"

struct ModelData;
struct TextureData;
struct Transform;
class Camera;

class Renderer
{
public:
	Renderer() = default;
	~Renderer() = default;

	void Init();
	void Render(ModelData& aModelData, TextureData& aTextureData, Transform& aTransform, std::shared_ptr<Camera> aCamera);

	void ChangeDebugMode(unsigned int aDebugMode);
	void ChangeBoneId(unsigned int aBoneId);
	bool ResetBoneId();
private:
	CBuffer<TransformBuffer> myCBufferTransform;

	CBuffer<DebugBuffer> myCBufferDebug;
	bool myResetBoneId = false;
};

