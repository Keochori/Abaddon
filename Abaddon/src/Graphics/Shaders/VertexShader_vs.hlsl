struct VS_INPUT 
{
	float4 position: POSITION;
	float4 position1: POSITION1;
	float4 color: COLOR;
	float4 color1: COLOR1;
};

struct VS_OUTPUT 
{
	float4 position: SV_POSITION;
	float4 color: COLOR;
	float4 color1: COLOR1;
};

cbuffer constant: register(b0) 
{
	row_major float4x4 transformation;
	unsigned int time;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	//output.position = lerp(input.position, input.position1, (sin(time/1000.0f) + 1.0f) / 2.0f);

	output.position = mul(input.position, transformation);
	output.color = input.color;	
	output.color1 = input.color1;	

	return output;
}