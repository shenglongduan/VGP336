// Description: Simple shader that does nothing.

float4 VS(float3 position : POSITION) : SV_Position
{
	return float4(position, 1.0f);
}

float4 PS(float4 position : SV_Position) : SV_Target
{
	return float4(1.0f, 1.0f, 0.0f, 1.0f); // Yellow
}