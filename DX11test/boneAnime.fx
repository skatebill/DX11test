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

cbuffer ConstantBuffer2 : register( b0 )
{
	matrix World;
	matrix View;
	matrix Projection;
}
cbuffer ConstantBuffer2 : register( b1 )
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
		dst +=  (mul(Pos,bone[4*i+0]) * input.boneplace[i].x);
		dst +=  (mul(Pos,bone[4*i+1]) * input.boneplace[i].y);
		dst +=  (mul(Pos,bone[4*i+2]) * input.boneplace[i].z);
		dst +=  (mul(Pos,bone[4*i+3]) * input.boneplace[i].w);
	}
	Pos = dst;
    Pos = mul( Pos, World );
    Pos = mul( Pos, View );
    Pos = mul( Pos, Projection );
	//output.Pos = float4(0,0,0,0);
	output.Pos = Pos;
	output.tex = input.tex;
	output.color = float4(input.boneplace[2].xyz,1);
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( VS_OUTPUT input ) : SV_Target
{
    return input.color;
}