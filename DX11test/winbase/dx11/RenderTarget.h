#pragma once
#include<d3d11.h>
#include<D3DX11.h>

class RenderTarget{
private:	
	ID3D11Texture2D* m_TexureBuffer;
	ID3D11RenderTargetView* m_TargetView;
	ID3D11ShaderResourceView* m_Resource;
public:
	RenderTarget();
	virtual ~RenderTarget();

	static RenderTarget* generateRenderTarget(ID3D11Device* devie,int w,int h,DXGI_FORMAT format=DXGI_FORMAT_R8G8B8A8_UNORM);

	bool generate(ID3D11Device* devie,int w,int h,DXGI_FORMAT format=DXGI_FORMAT_R8G8B8A8_UNORM);
	void release();
	ID3D11RenderTargetView** getTarget(){return &m_TargetView;}
	ID3D11ShaderResourceView** getShaderResource(){return &m_Resource;}
	void useTarget(ID3D11DeviceContext* context,ID3D11DepthStencilView* depthView=0){context->OMSetRenderTargets(1,&m_TargetView,depthView);}
};