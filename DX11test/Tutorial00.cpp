//--------------------------------------------------------------------------------------
// File: Tutorial01.cpp
//
// This application demonstrates creating a Direct3D 11 device
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "DX11WBASE.h"
class myRender:public DX11WBASE
{

public:
	myRender(HINSTANCE hInstance):DX11WBASE(hInstance){};
	virtual ~myRender(){}
	void render(float delta)
	{
		float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 0.0f }; //red,green,blue,alpha
		m_pImmediateContext->ClearRenderTargetView( m_pRenderTargetView, ClearColor );




		m_pSwapChain->Present( 0, 0 );

	}
	void intiData(){
	
	}
	void cleanup(){

	}
};


int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	myRender t(hInstance);
	t.createWindow(L"testwindow",800,600);
	t.run();
    return 0;
}
