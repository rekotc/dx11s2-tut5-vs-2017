////////////////////////////////////////////////////////////////////////////////
// Filename: model.vs
////////////////////////////////////////////////////////////////////////////////

//////////////
// TYPEDEFS //
//////////////

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 worldPos : POSITION;
    float2 TexCoord : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

/////////////
// GLOBALS //
/////////////
cbuffer cbPerObject
{
    float4x4 WVP;
    float4x4 World;

    float4 difColor;
    bool hasTexture;
    bool hasNormMap;
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////

VS_OUTPUT ModelVertexShader(float4 inPos : POSITION, float2 inTexCoord : TEXCOORD, float3 normal : NORMAL, float3 tangent : TANGENT)
{
    VS_OUTPUT output;
    
    output.Pos = mul(inPos, WVP);
    output.worldPos = mul(inPos, World);

    output.normal = mul(normal, World);

    output.tangent = mul(tangent, World);

    output.TexCoord = inTexCoord;

    return output;
}

