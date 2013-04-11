#pragma once

#include <d3dx11.h>
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

class ShaderProgram
{
private:
	ID3D11Device*           m_pd3dDevice;

	ID3D11VertexShader*     m_pVertexShader;
	ID3D11PixelShader*      m_pPixelShader;
	ID3D11InputLayout*      m_pVertexLayout;

	D3D11_INPUT_ELEMENT_DESC*	m_layout;
	int							m_numElemnts;
public:
	ShaderProgram(ID3D11Device* device):m_layout(0),m_pVertexLayout(0),m_pVertexShader(0),m_pPixelShader(0){m_pd3dDevice=device;}
	virtual ~ShaderProgram(void){}
	
	ID3D11VertexShader*	getVertexShader(){return m_pVertexShader;}
	ID3D11PixelShader* getPixelShader(){return m_pPixelShader;}
	ID3D11InputLayout* getVertexLayout(){return m_pVertexLayout;}
	void	useLayout(ID3D11DeviceContext* pContext){pContext->IASetInputLayout(getVertexLayout());}

	virtual HRESULT loadShader(WCHAR* fileName);
	virtual void setLayout(D3D11_INPUT_ELEMENT_DESC* layout,int num){m_layout=layout;m_numElemnts=num;}
};

