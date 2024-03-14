struct VS_INPUT 
{
	float4 position: POSITION;
	float2 texcoord: TEXCOORD;
};

struct VS_OUTPUT 
{
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD;
};

cbuffer transformBuffer: register(b0) 
{
	float4x4 transformation;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.position = mul(input.position, transformation);
	output.texcoord = input.texcoord;	

	return output;
}