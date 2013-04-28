#pragma once
#include "../windowcontroller.h"
#include <d3dx11.h>
#pragma comment(lib,"d3d11.lib")
typedef enum DrawMode{
	Triangle,
	Line
}DrawMode;
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

	
	virtual void render(float delta)=0;
	virtual void intiData()=0;
	virtual void cleanup()=0;
};