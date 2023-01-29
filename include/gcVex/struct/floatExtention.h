#pragma onec

struct float2
{
    float x;
    float y;

    float2(){
        x = 0;
        y = 0;
    }
    float2(float xVal, float yVal){
        x = xVal;
        y = yVal;
    }
};

struct float3
{
    float x;
    float y;
    float z;

    float3(){
        x = 0;
        y = 0;
        z = 0;
    }
    float3(float xVal, float yVal, float zVal){
        x = xVal;
        y = yVal;
        z = zVal;
    }
};