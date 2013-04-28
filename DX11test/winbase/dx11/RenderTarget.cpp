#pragma once
#include"RenderTarget.h"

RenderTarget::RenderTarget()
{	
	m_TexureBuffer=0;
	m_TargetView=0;
	m_Resource=0;
}



RenderTarget::~RenderTarget(){

}

RenderTarget* RenderTarget::generateRenderTarget(ID3D11Device* device,int w,int h,DXGI_FORMAT format){
	RenderTarget* result=new RenderTarget();
	if(result->generate(device,w,h,format))
		return result;
	delete result;
	return 0;
}

bool RenderTarget::generate(ID3D11Device* device,int w,int h,DXGI_FORMAT format){
	
	D3D11_TEXTURE2D_DESC rttdesc;
	ZeroMemory(&rttdesc,sizeof(rttdesc));
	rttdesc.Width=w;
	rttdesc.Height=h;
	rttdesc.Format=format;
	rttdesc.CPUAccessFlags=0;
	rttdesc.BindFlags=D3D11_BIND_RENDER_TARGET|D3D11_BIND_SHADER_RESOURCE;
	rttdesc.ArraySize=1;
	rttdesc.Usage=D3D11_USAGE_DEFAULT;
	rttdesc.SampleDesc.Count=1;
	rttdesc.SampleDesc.Quality=0;
	HRESULT hr=device->CreateTexture2D(&rttdesc,0,&m_TexureBuffer);
	if(FAILED(hr))
		return false;


	D3D11_SHADER_RESOURCE_VIEW_DESC srdesc;
	ZeroMemory(&srdesc,sizeof(srdesc));
	srdesc.Texture2D.MipLevels=1;
	srdesc.Texture2D.MostDetailedMip=0;
	srdesc.Format=format;
	srdesc.ViewDimension=D3D11_SRV_DIMENSION_TEXTURE2D;
	hr=device->CreateShaderResourceView(m_TexureBuffer,&srdesc,&m_Resource);
	if(FAILED(hr))
		return false;


	D3D11_RENDER_TARGET_VIEW_DESC rtvdesc;
	ZeroMemory(&rtvdesc,sizeof(rtvdesc));
	rtvdesc.Format=format;
	rtvdesc.ViewDimension=D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvdesc.Texture2D.MipSlice=0;
	hr=device->CreateRenderTargetView(m_TexureBuffer,&rtvdesc,&m_TargetView);
	if(FAILED(hr))
		return false;
	return true;
}

void RenderTarget::release(){
	if(m_TexureBuffer)
		m_TexureBuffer->Release();
	if(m_TargetView)
		m_TargetView->Release();
	if(m_Resource)
		m_Resource->Release();
	m_TexureBuffer=0;
	m_TargetView=0;
	m_Resource=0;
}