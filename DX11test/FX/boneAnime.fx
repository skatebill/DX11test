//--------------------------------------------------------------------------------------
// File: Tutorial04.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D txDiffuse : register( t0 );
SamplerState samLinear : register( s0 );

cbuffer ConstantBuffer : register( b0 )
{
	matrix World;
	matrix View;
	matrix Projection;
}
cbuffer ConstantBuffer : register( b1 )
{
	matrix bone[40];
}
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos : SV_POSITION;	
    float2 tex : TEXCOORD;
	float4 boneplace[10] : BONE;
};
//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float2 tex : TEXCOORD;
	float4 color : COLOR;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
	float4 Pos = input.Pos;
	float4 dst = float4(0,0,0,0);
	for(int i=0;i<10;i++)
	{
		float b1=input.boneplace[i].x;
		float b2=input.boneplace[i].y;
		float b3=input.boneplace[i].z;
		float b4=input.boneplace[i].w;
		dst +=  mul(Pos,bone[4*i+0]) * b1;
		dst +=  mul(Pos,bone[4*i+1]) * b2;
		dst +=  mul(Pos,bone[4*i+2]) * b3;
		dst +=  mul(Pos,bone[4*i+3]) * b4;
	}
	Pos = dst;
    Pos = mul( Pos, World );
    Pos = mul( Pos, View );
    Pos = mul( Pos, Projection );
	output.Pos = Pos;
	output.tex = input.tex;
	output.color = float4(1,1,1,1);
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( VS_OUTPUT input ) : SV_Target
{
    return txDiffuse.Sample( samLinear, input.tex );
}
