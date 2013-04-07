#pragma once

#include <d3dx11.h>
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DX11
//--------------------------------------------------------------------------------------
HRESULT compileShaderFromFile( WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut );
class MyShaderPrograme
{
private:
	ID3D11Device*           m_pd3dDevice;

	ID3D11VertexShader*     m_pVertexShader;
	ID3D11PixelShader*      m_pPixelShader;
	ID3D11InputLayout*      m_pVertexLayout;
public:
	MyShaderPrograme(ID3D11Device* device);
	virtual ~MyShaderPrograme(void);

	bool loadShader(WCHAR* fileName);
	ID3D11VertexShader*	getVertexShader(){return m_pVertexShader;}
		ID3D11PixelShader* getPixelShade(){return m_pPixelShader;}
		ID3D11InputLayout* getVertexLayout(){return m_pVertexLayout;}
};

