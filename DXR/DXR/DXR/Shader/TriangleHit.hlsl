#include "TriangleHit.hlsli"
#include "Infomation.hlsli"

// �N���[�[�X�g�q�b�g
[shader("closesthit")]
void TriangleHit(inout Payload payload, in BuiltInTriangleIntersectionAttributes attribute)
{
    payload.hit = true;
}