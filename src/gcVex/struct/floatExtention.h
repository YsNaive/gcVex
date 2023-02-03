#ifndef GCVEX_FLOATECTENTION_H_
#define GCVEX_FLOATECTENTION_H_

struct float2
{
    float x;
    float y;

    float2();
    float2(float xVal, float yVal);
};

struct float3
{
    float x;
    float y;
    float z;

    float3();
    float3(float xVal, float yVal, float zVal);
    
    float3 operator + (float3 const &f3);
    float3 operator - (float3 const &f3);
    float3 operator * (float3 const &f3);
    float3 operator / (float3 const &f3);
    float3 operator * (float const &f);
    float3 operator / (float const &f);

    float sum();
};

#endif
