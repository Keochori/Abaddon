struct VS_INPUT 
{
	float4 position: POSITION;
	float2 texcoord: TEXCOORD;
	uint4 boneids: BONEIDS;
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
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	float4 worldPosition = mul(transformationMatrix, input.position);
	float4 viewPosition = mul(viewMatrix, worldPosition);
	float4 projectionPosition = mul(projectionMatrix, viewPosition);	

	output.position = projectionPosition;
	output.texcoord = input.texcoord;	
	output.boneids = input.boneids;
	output.boneweights = input.boneweights;

	return output;
}