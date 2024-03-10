cbuffer transformBuffer: register(b0) 
{
	float4x4 transformation;
};

float4 main(float4 position : POSITION) : SV_POSITION
{
	position = mul(position, transformation);

	return position;
}