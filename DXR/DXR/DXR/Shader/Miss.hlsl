#include "Miss.hlsli"
#include "Infomation.hlsli"

// �~�X
[shader("miss")]
void Miss(inout Payload payload)
{
    payload.hit = false;
}