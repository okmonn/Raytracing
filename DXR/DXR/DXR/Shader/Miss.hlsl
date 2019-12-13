#include "Miss.hlsli"
#include "Infomation.hlsli"
#include "GlobalInfo.hlsl"

// É~ÉX
[shader("miss")]
void Miss(inout Payload payload)
{
    payload.color = float3(0.4f, 0.6f, 0.2f);
}