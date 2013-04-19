#pragma once

#include <d3dx11.h>
#include <d3dcompiler.h>
#include <map>
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
	std::map<int,ID3D11Buffer*>	m_bufferMap;
	ShaderProgram(ID3D11Device* device):m_layout(0),m_pVertexLayout(0),m_pVertexShader(0),m_pPixelShader(0){m_pd3dDevice=device;}
	virtual ~ShaderProgram(void){}
	
	ID3D11VertexShader*	getVertexShader(){return m_pVertexShader;}
	ID3D11PixelShader* getPixelShader(){return m_pPixelShader;}
	ID3D11InputLayout* getVertexLayout(){return m_pVertexLayout;}
	void	useLayout(ID3D11DeviceContext* pContext){pContext->IASetInputLayout(getVertexLayout());}
	void	setConstantBuffer(ID3D11DeviceContext* pContext,int slot,void* buffer);
	bool	requestConstantBuffer(int size,int slot=-1){
		if(slot==-1)
			slot=m_bufferMap.size();

		ID3D11Buffer* constantBuffer=0;
		D3D11_BUFFER_DESC bd;
		ZeroMemory( &bd, sizeof(bd) );
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = size;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		HRESULT hr = m_pd3dDevice->CreateBuffer( &bd, NULL, &constantBuffer );
		if( FAILED( hr ) )
			return false;
		
		m_bufferMap[slot]=constantBuffer;
		return true;

	};

	virtual HRESULT loadShader(WCHAR* fileName);
	virtual void setLayout(D3D11_INPUT_ELEMENT_DESC* layout,int num){m_layout=layout;m_numElemnts=num;}
};

