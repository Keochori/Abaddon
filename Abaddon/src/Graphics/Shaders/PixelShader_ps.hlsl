struct PS_INPUT 
{
	float4 position: SV_POSITION;
	float4 color: COLOR;
	float4 color1: COLOR1;
};

cbuffer constant: register(b0) 
{
	unsigned int time;
};

float4 main(PS_INPUT input) : SV_TARGET
{
	//input.color = lerp(input.color, input.color1, (sin(time/1000.0f) + 1.0f) / 2.0f);
	return input.color;
}