#include "TriangleHit.hlsli"
#include "Infomation.hlsli"

// クローゼストヒット
[shader("closesthit")]
void TriangleHit(inout Payload payload, in BuiltInTriangleIntersectionAttributes attribute)
{
    float3 barycentrics = float3(1.0 - attribute.barycentrics.x - attribute.barycentrics.y, attribute.barycentrics.x, attribute.barycentrics.y);

    const float3 A = float3(1, 0, 0);
    const float3 B = float3(0, 1, 0);
    const float3 C = float3(0, 0, 1);

    payload.color = A * barycentrics.x + B * barycentrics.y + C * barycentrics.z;
}