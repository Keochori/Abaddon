#include "pch.h"
#include "Renderer.h"
#include "Graphics/Bindables/Bindables.h"
#include "Scene/Transform.h"
#include "Scene/Camera.h"

#define DegToRadFactor 0.01745329251f

void Renderer::Init()
{
	myCBufferTransform.Init(eBindType::VertexShader);
	myCBufferDebug.Init(eBindType::VsAndPs);
}

void Renderer::Render(ModelData& aModelData, TextureData& aTextureData, Transform& aTransform, std::shared_ptr<Camera> aCamera)
{
	// Bind model
	aModelData.myVertexBuffer.Bind();
	aModelData.myIndexBuffer.Bind();
	aModelData.myInputLayout.Bind();

	// Bind texture
	aTextureData.mySRV.Bind();
	aTextureData.mySampler.Bind();

	// Convert rotation from degrees to radians
	aTransform.myRotation *= DegToRadFactor;

	// Transformation buffer
	myCBufferTransform.myData.myTransformationMatrix =
			DirectX::XMMatrixRotationRollPitchYaw(aTransform.myRotation.x, aTransform.myRotation.y, aTransform.myRotation.z) *
			DirectX::XMMatrixTranslation(aTransform.myPosition.x, aTransform.myPosition.y, aTransform.myPosition.z) *
			DirectX::XMMatrixScaling(aTransform.myScale.x, aTransform.myScale.y, aTransform.myScale.z);

	myCBufferTransform.myData.myViewMatrix = aCamera->GetMatrix();
	myCBufferTransform.myData.myProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH(1.0f, 16.0f / 9.0f, 0.1f, 1000.0f);

	myCBufferTransform.ApplyChanges();
	myCBufferTransform.Bind();

	// Debug Mode buffer
	myResetBoneId = false;
	if (myCBufferDebug.myData.myBoneId == aModelData.mySkeleton.myBoneAmount)
		myResetBoneId = true;
	myCBufferDebug.ApplyChanges();
	myCBufferDebug.Bind(1);
	LOG(std::to_string(myCBufferDebug.myData.myBoneId));

	// Draw textured model
	DX11::ourContext->DrawIndexed(aModelData.myIndexBuffer.GetIndexAmount(), 0, 0);
}

void Renderer::ChangeDebugMode(unsigned int aDebugMode)
{
	myCBufferDebug.myData.myDebugMode = aDebugMode;
}

void Renderer::ChangeBoneId(unsigned int aBoneId)
{
	myCBufferDebug.myData.myBoneId = aBoneId;
}

bool Renderer::ResetBoneId()
{
	return myResetBoneId;
}
