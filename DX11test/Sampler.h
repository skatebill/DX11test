#pragma once
#include "MyResource.h"
#include<D3DX11.h>
class Sampler:public MyResource
{
private:
	
	ID3D11Device *m_pd3dDevice;
	ID3D11SamplerState* m_pSampler;
public:
	Sampler(ID3D11Device*);
	~Sampler(void);
	
	bool createSampleState(D3D11_FILTER,D3D11_TEXTURE_ADDRESS_MODE);
	ID3D11SamplerState* getSampleState();
	void release();
	bool isReleased();
	void useSamplerAt(ID3D11DeviceContext*,int);
};

