#include "DX11WBASE.h"


DX11WBASE::DX11WBASE(HINSTANCE instance):WindowController(instance)
{
	
	D3D_DRIVER_TYPE         m_driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL       m_featureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11Device*           m_pd3dDevice = NULL;
	ID3D11DeviceContext*    m_pImmediateContext = NULL;
	IDXGISwapChain*         m_pSwapChain = NULL;
	ID3D11RenderTargetView* m_pRenderTargetView = NULL;
}


DX11WBASE::~DX11WBASE(void)
{
	CleanupDevice();
}

HRESULT DX11WBASE::createWindow(LPCWSTR title,int w,int h,bool fullscreen)
{
	if(FAILED(WindowController::createWindow(title,w,h,fullscreen))){
		return E_FAIL;
	}

	
    HRESULT hr = S_OK;

    RECT rc;
    GetClientRect( m_hWnd, &rc );
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE
    };
    UINT numDriverTypes = ARRAYSIZE( driverTypes );

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_10_0
    };
	UINT numFeatureLevels = ARRAYSIZE( featureLevels );

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory( &sd, sizeof( sd ) );
    sd.BufferCount = 1;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = m_hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = fullscreen? FALSE:TRUE;

    for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
    {
        m_driverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDeviceAndSwapChain( NULL, m_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
                                            D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pd3dDevice, &m_featureLevel, &m_pImmediateContext );
        if( SUCCEEDED( hr ) )
            break;
    }
    if( FAILED( hr ) )
        return hr;

    // Create a render target view
    ID3D11Texture2D* pBackBuffer = NULL;
    hr = m_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
    if( FAILED( hr ) )
        return hr;

    hr = m_pd3dDevice->CreateRenderTargetView( pBackBuffer, NULL, &m_pRenderTargetView );
    pBackBuffer->Release();
    if( FAILED( hr ) )
        return hr;

    m_pImmediateContext->OMSetRenderTargets( 1, &m_pRenderTargetView, NULL );

    // Setup the viewport
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)width;
    vp.Height = (FLOAT)height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    m_pImmediateContext->RSSetViewports( 1, &vp );
	
	m_pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
    return S_OK;

}


void DX11WBASE::CleanupDevice(){

    if( m_pImmediateContext ) m_pImmediateContext->ClearState();
    if( m_pRenderTargetView ) m_pRenderTargetView->Release();
    if( m_pSwapChain ) m_pSwapChain->Release();
    if( m_pImmediateContext ) m_pImmediateContext->Release();
    if( m_pd3dDevice ) m_pd3dDevice->Release();

}
void DX11WBASE::setDrawMode(DrawMode model){
	D3D11_RASTERIZER_DESC rasterDesc;

	rasterDesc.AntialiasedLineEnable = false;  
	rasterDesc.CullMode = D3D11_CULL_BACK;  
	rasterDesc.DepthBias = 0;  
	rasterDesc.DepthBiasClamp = 0.0f;  
	rasterDesc.DepthClipEnable = true;  
	rasterDesc.FrontCounterClockwise = false;  
	rasterDesc.MultisampleEnable = false;  
	rasterDesc.ScissorEnable = false;  
	rasterDesc.SlopeScaledDepthBias = 0.0f;  
	ID3D11RasterizerState* state;
	switch(model){
	case Triangle:
		rasterDesc.FillMode = D3D11_FILL_SOLID;  
		break;
	case Line:
		rasterDesc.FillMode = D3D11_FILL_WIREFRAME;  
		break;
	default:
		break;
	}


	m_pd3dDevice->CreateRasterizerState(&rasterDesc,&state);
	m_pImmediateContext->RSSetState(state);
}