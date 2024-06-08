//const int MAX_BONES = 128;
//const int MAX_WEIGHTS = 4;

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
};

cbuffer transformBuffer: register(b0) 
{
	float4x4 transformation;
	float4x4 boneData[128];
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	float4 totalLocalPos = float4(0, 0, 0, 0);

	float4x4 boneTransform1 = boneData[input.boneids.x];
	float4 posePosition1 = mul(boneTransform1, input.position);
	totalLocalPos += posePosition1 * input.boneweights.x;

	float4x4 boneTransform2 = boneData[input.boneids.y];
	float4 posePosition2 = mul(boneTransform2, input.position);
	totalLocalPos += posePosition2 * input.boneweights.y;

	float4x4 boneTransform3 = boneData[input.boneids.z];
	float4 posePosition3 = mul(boneTransform3, input.position);
	totalLocalPos += posePosition3 * input.boneweights.z;

	float4x4 boneTransform4 = boneData[input.boneids.w];
	float4 posePosition4 = mul(boneTransform4, input.position);
	totalLocalPos += posePosition4 * input.boneweights.w;


	output.position = mul(totalLocalPos, transformation);
	output.texcoord = input.texcoord;

	return output;
}