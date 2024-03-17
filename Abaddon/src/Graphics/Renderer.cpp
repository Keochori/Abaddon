#include "pch.h"
#include "Renderer.h"
#include "DX11.h"

void Renderer::Render(std::vector<std::shared_ptr<Model>> aModelList)
{
	for (int mIndex = 0; mIndex < aModelList.size(); mIndex++)
	{
		ModelData bindables = aModelList[mIndex]->GetModelData();
		bindables.myVertexBuffer.Bind();
		bindables.myIndexBuffer.Bind();
		bindables.myInputLayout.Bind();
		bindables.mySRV.Bind();
		bindables.mySampler.Bind();
		bindables.myTransformCBuffer.Bind();

		// Draw model
		DX11::ourContext->DrawIndexed(aModelList[mIndex]->GetModelData().myIndexBuffer.GetIndexAmount(), 0, 0);
	}
}
