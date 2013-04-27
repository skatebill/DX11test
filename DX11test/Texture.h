#pragma once
#include "myresource.h"
#include <Windows.h>
#include <d3d11.h>
class Texture :
	public MyResource
{
public:
	Texture(void);
	~Texture(void);

	virtual bool loadFromFile(LPWCH,ID3D11DeviceContext* context)=0;
	virtual ID3D11ShaderResourceView* getTexture()=0;
};

