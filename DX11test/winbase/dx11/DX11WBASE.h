#pragma once
#include "../windowcontroller.h"
#include <d3dx11.h>
#pragma comment(lib,"d3d11.lib")
typedef enum DrawMode{
	Triangle,
	Line
}DrawMode;
static ID3D11ShaderResourceView* pSRV[] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };

class DX11WBASE :
	public WindowController
{
protected:
	
	D3D_DRIVER_TYPE         m_driverType;
	D3D_FEATURE_LEVEL       m_featureLevel;
	ID3D11Device*           m_pd3dDevice;
	ID3D11DeviceContext*    m_pImmediateContext;
	IDXGISwapChain*         m_pSwapChain;
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11DepthStencilView* m_depthStencilView;

public:
	DX11WBASE(HINSTANCE instance);
	virtual ~DX11WBASE(void);
	
	HRESULT createWindow(LPCWSTR title,int w=800,int h=600,bool fullscreen=false);

	HRESULT initDevice();
	void CleanupDevice();
	void setDrawMode(DrawMode model);
	void presentDraw(){m_pSwapChain->Present(0,0);clearResources();}

	void clearTargetColor(ID3D11RenderTargetView* dstView,float* color){m_pImmediateContext->ClearRenderTargetView(dstView,color);}
	void clearTargetDepth(ID3D11DepthStencilView* dstView,float clearnum){m_pImmediateContext->ClearDepthStencilView(dstView,D3D11_CLEAR_DEPTH,clearnum,0);}
	void clearTargetStencil(ID3D11DepthStencilView* dstView,UINT8 clearnum){m_pImmediateContext->ClearDepthStencilView(dstView,D3D11_CLEAR_STENCIL,0,clearnum);}
	void clearResources(){m_pImmediateContext->PSSetShaderResources( 0, _countof(pSRV), pSRV );}
	
	virtual void render(float delta)=0;
	virtual void intiData()=0;
	virtual void cleanup()=0;
};