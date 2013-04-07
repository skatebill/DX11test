//--------------------------------------------------------------------------------------
// File: Tutorial01.cpp
//
// This application demonstrates creating a Direct3D 11 device
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"d3dx11.lib")

#include "DX11WBASE.h"
#include "MyShaderPrograme.h"
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>


ID3D11VertexShader*     m_pVertexShader = NULL;
ID3D11PixelShader*      m_pPixelShader = NULL;
ID3D11InputLayout*      m_pVertexLayout = NULL;
ID3D11Buffer*           m_pVertexBuffer = NULL;
MyShaderPrograme *shader=0;

//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DX11
//--------------------------------------------------------------------------------------
HRESULT CompileShaderFromFile( WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut )
{
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* pErrorBlob;
    hr = D3DX11CompileFromFile( szFileName, NULL, NULL, szEntryPoint, szShaderModel, 
        dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL );
    if( FAILED(hr) )
    {
        if( pErrorBlob != NULL )
            OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
        if( pErrorBlob ) pErrorBlob->Release();
        return hr;
    }
    if( pErrorBlob ) pErrorBlob->Release();
    return S_OK;
}

class myRender:public DX11WBASE
{

public:
	myRender(HINSTANCE hInstance):DX11WBASE(hInstance){};
	virtual ~myRender(){}
	void render(float delta)
	{
		float ClearColor[4] = { 1.0f, 0.125f, 0.3f, 1.0f }; //red,green,blue,alpha
		m_pImmediateContext->ClearRenderTargetView( m_pRenderTargetView, ClearColor );

		
		// Render a triangle
		m_pImmediateContext->VSSetShader( m_pVertexShader, NULL, 0 );
		m_pImmediateContext->PSSetShader( m_pPixelShader, NULL, 0 );
		m_pImmediateContext->Draw( 3, 0 );



		m_pSwapChain->Present( 0, 0 );

	}
	struct SimpleVertex
	{
    XMFLOAT3 Pos;
	};

	void intiData(){
	
	HRESULT hr=0;
	shader=new MyShaderPrograme(m_pd3dDevice);
	bool result=shader->loadShader(L"Tutorial02.fx");
	if(!result)
	{
		return;
	}

    // Set the input layout
	m_pImmediateContext->IASetInputLayout( shader->getVertexLayout() );
	m_pVertexShader = shader->getVertexShader();
	m_pPixelShader = shader->getPixelShade();

    SimpleVertex vertices[] =
    {
        XMFLOAT3( 0.0f, 0.5f, 0.5f ),
        XMFLOAT3( 0.5f, -0.5f, 0.5f ),
        XMFLOAT3( -0.5f, -0.5f, 0.5f ),
    };
    D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof(bd) );
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( SimpleVertex ) * 3;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory( &InitData, sizeof(InitData) );
    InitData.pSysMem = vertices;
    hr = m_pd3dDevice->CreateBuffer( &bd, &InitData, &m_pVertexBuffer );
    // Set vertex buffer
    UINT stride = sizeof( SimpleVertex );
    UINT offset = 0;
    m_pImmediateContext->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );

    // Set primitive topology
    m_pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	}
	void cleanup(){
		
    if( m_pVertexBuffer ) m_pVertexBuffer->Release();
    if( m_pVertexLayout ) m_pVertexLayout->Release();
    if( m_pVertexShader ) m_pVertexShader->Release();
	}
};


int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	myRender t(hInstance);
	t.createWindow(L"testwindow",800,600);
	t.run();
    return 0;
}
