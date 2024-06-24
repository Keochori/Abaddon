Texture2D Texture: register(t0);
sampler TextureSampler: register(s0);

struct PS_INPUT 
{
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD;
	uint4 boneids: BONEIDS;
	float4 boneweights: BONEWEIGHTS;
};

cbuffer debugBuffer: register(b1) 
{
	uint debugMode;
	uint boneId;
};

float4 main(PS_INPUT input) : SV_TARGET
{
	if (debugMode == 1) 
	{
		return Texture.Sample(TextureSampler, input.texcoord);
	}
	
	if (debugMode == 3) 
	{
		float boneWeight = 0.0f;
		
		for (int i = 0; i < 4; i++) 
		{
			if (boneId == input.boneids[i]) 
			{
				boneWeight = input.boneweights[i];
			}
		}
	
		return float4(boneWeight,0.0f,1.0f - boneWeight,1.0f);
	}


	return float4(0.0f,0.0f,0.0f,0.0f);
}