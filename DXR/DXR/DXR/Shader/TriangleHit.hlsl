#include "TriangleHit.hlsli"
#include "Infomation.hlsli"

// クローゼストヒット
[shader("closesthit")]
void TriangleHit(inout Payload payload, in BuiltInTriangleIntersectionAttributes attribute)
{
    payload.hit = true;
}