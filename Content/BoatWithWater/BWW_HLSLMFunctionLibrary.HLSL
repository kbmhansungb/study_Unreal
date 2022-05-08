
// How to use : https://github.com/Phyronnaz/HLSLMaterial

// Ray-sphere intersection
// @param   RayOrigin       The origin of the ray
// @param   RayDirection    The direction of the ray
// @param   SphereCenter    The center of the sphere
// @param   SphereRadius    The radius of the sphere
// @param   Distance        The distance from the ray origin to the hit on the sphere
void RaySphereIntersect(float3 RayOrigin, float3 RayDirection, float3 SphereCenter, float SphereRadius, out float Distance)
{
    float a = dot(RayDirection, RayDirection);
    float3 SphereCenterToRayOrigin = RayOrigin - SphereCenter;
    float b = 2.0 * dot(RayDirection, SphereCenterToRayOrigin);
    float c = dot(SphereCenterToRayOrigin, SphereCenterToRayOrigin) - (SphereRadius * SphereRadius);
    float Discriminant = b * b - 4.0 * a * c;

    if (Discriminant < 0.0)
    {
        Distance = -1.0;
    }
    else
    {
        Distance = (-b - sqrt(Discriminant)) / 2 * a;
    }
}


void NormalsEdgeAttenuation()
{
}