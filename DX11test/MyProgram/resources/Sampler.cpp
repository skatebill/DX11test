#include "Sampler.h"


Sampler::Sampler(ID3D11Device* device):m_pSampler(0),MyResource()
{
	m_pd3dDevice=device;
}


Sampler::~Sampler(void)
{
	Release();
}


ID3D11SamplerState* Sampler::getSampleState(){
	return m_pSampler;
}
void Sampler::Release(){
	if(m_pSampler)
		m_pSampler->Release();
	m_pSampler=0;
}
bool Sampler::isReleased(){
	return m_pSampler==0;
}
void Sampler::useSamplerAt(ID3D11DeviceContext* context,int slot){
	context->PSSetSamplers( 0, 1, &m_pSampler );

}
bool Sampler::createSampleState(D3D11_FILTER f,D3D11_TEXTURE_ADDRESS_MODE m,float mipOffset){
		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory( &sampDesc, sizeof(sampDesc) );
		sampDesc.Filter = f;
		sampDesc.AddressU = m;
		sampDesc.AddressV = m;
		sampDesc.AddressW = m;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		sampDesc.MipLODBias = mipOffset;
		HRESULT hr = m_pd3dDevice->CreateSamplerState( &sampDesc, &m_pSampler );
		if( FAILED( hr ) )
			return false;
		return true;

}
