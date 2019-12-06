#include "Miss.hlsli"
#include "Infomation.hlsli"

// ƒ~ƒX
[shader("miss")]
void Miss(inout Payload payload)
{
    payload.hit = false;
}