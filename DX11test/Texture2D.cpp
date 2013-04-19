#include "Texture2D.h"


Texture2D::Texture2D(ID3D11Device *device):m_RecourceView(0),Texture()
{
	m_pd3dDevice=device;
}


Texture2D::~Texture2D(void)
{
}


bool Texture2D::loadFromFile(LPWCH filename){
	HRESULT hr = D3DX11CreateShaderResourceViewFromFile( m_pd3dDevice,filename, NULL, NULL, 
         &m_RecourceView,NULL);
	return !FAILED(hr);
}

bool Texture2D::loadFromFile(char* filename){
	int num=MultiByteToWideChar(CP_ACP,0,filename,-1,NULL,0);
	LPWCH wfilename=new WCHAR[num];
	MultiByteToWideChar(CP_ACP,0,filename,-1,wfilename,num);
	HRESULT hr = D3DX11CreateShaderResourceViewFromFile( m_pd3dDevice,wfilename, NULL, NULL, 
         &m_RecourceView,NULL);
	delete wfilename;
	return !FAILED(hr);
}
ID3D11ShaderResourceView* Texture2D::getTexture(){
	return m_RecourceView;
}
void Texture2D::release(){
	if(m_RecourceView)
		m_RecourceView->Release();
	m_RecourceView = 0;

}
bool Texture2D::isReleased(){
	return m_RecourceView == 0;
}
void Texture2D::useTextureAt(ID3D11DeviceContext* context,int slot){
	context->PSSetShaderResources(slot,1,&m_RecourceView);
}