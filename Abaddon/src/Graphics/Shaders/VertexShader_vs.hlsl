#define MAX_BONES 128
#define MAX_WEIGHTS 4

struct VS_INPUT 
{
	float4 position: POSITION;
	float2 texcoord: TEXCOORD;
	int4 boneids: BONEIDS;
	float4 boneweights: BONEWEIGHTS;
};

struct VS_OUTPUT 
{
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD;
	uint4 boneids: BONEIDS;
	float4 boneweights: BONEWEIGHTS;
};

cbuffer transformBuffer: register(b0) 
{
	float4x4 transformationMatrix;
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
	float4x4 boneTransforms[MAX_BONES];
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	float4x4 boneTransform;
	for (int i = 0; i < MAX_WEIGHTS; i++) 
	{
		if (input.boneweights[i] == 0 || input.boneids[i] == -1) 
			continue;

		boneTransform += boneTransforms[input.boneids[i]] * input.boneweights[i];
	}

	float4 localPosition = mul(boneTransform, input.position);
	float4 worldPosition = mul(transformationMatrix, localPosition);
	float4 viewPosition = mul(viewMatrix, worldPosition);
	float4 projectionPosition = mul(projectionMatrix, viewPosition);	

	output.position = projectionPosition;
	output.texcoord = input.texcoord;	
	output.boneids = input.boneids;
	output.boneweights = input.boneweights;

	return output;
}