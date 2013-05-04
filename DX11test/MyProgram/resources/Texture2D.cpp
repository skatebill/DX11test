#include "Texture2D.h"


Texture2D::Texture2D(ID3D11Device *device):m_RecourceView(0),Texture(),m_SelfGenerateFlag(false)
{
	m_pd3dDevice=device;
}


Texture2D::~Texture2D(void)
{
}


bool Texture2D::loadFromFile(LPWCH filename,ID3D11DeviceContext* context){
	HRESULT hr = 0;
	D3DX11_IMAGE_INFO imageInfo;
	hr=D3DX11GetImageInfoFromFile(filename,0,&imageInfo,0);
	if(FAILED(hr))
	{
		return false;
	}
	int width=imageInfo.Width;
	int height=imageInfo.Height;
	int numMipmap=0;
	while(width&&height)
	{
		height=height>>1;
		width=width>>1;
		numMipmap++;
	}
	D3DX11_IMAGE_LOAD_INFO info;
	info.Format=DXGI_FORMAT_R32G32B32A32_FLOAT;
	info.MipLevels=D3DX11_DEFAULT;
	info.Filter=D3DX11_FILTER_LINEAR;
	info.BindFlags=D3D11_BIND_SHADER_RESOURCE;
	info.MiscFlags|=D3D11_RESOURCE_MISC_GENERATE_MIPS;
	info.pSrcInfo=&imageInfo;
	hr = D3DX11CreateShaderResourceViewFromFile( m_pd3dDevice,filename, &info, NULL, 
         &m_RecourceView,NULL);
	if(FAILED(hr))
	{
		return false;
	}
	//context->GenerateMips(m_RecourceView);

#ifdef _DEBUG
    ID3D11Texture2D* tex; 
    m_RecourceView->GetResource((ID3D11Resource**)&tex); 
    D3D11_TEXTURE2D_DESC texdesc; 
    tex->GetDesc(&texdesc);
#endif
	m_SelfGenerateFlag=true;
    return true; 
}

bool Texture2D::loadFromFile(char* filename,ID3D11DeviceContext* context){
	int num=MultiByteToWideChar(CP_ACP,0,filename,-1,NULL,0);
	LPWCH wfilename=new WCHAR[num];
	MultiByteToWideChar(CP_ACP,0,filename,-1,wfilename,num);
	bool result = loadFromFile(wfilename,context);	
	delete wfilename;
	return result;
}
ID3D11ShaderResourceView* Texture2D::getTexture(){
	return m_RecourceView;
}
void Texture2D::Release(){
	if(m_RecourceView&&m_SelfGenerateFlag)
		m_RecourceView->Release();
	m_RecourceView = 0;

}
bool Texture2D::isReleased(){
	return m_RecourceView == 0;
}
void Texture2D::useTextureAt(ID3D11DeviceContext* context,int slot){
	context->PSSetShaderResources(slot,1,&m_RecourceView);
}