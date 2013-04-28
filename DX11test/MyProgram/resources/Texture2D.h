#pragma once
#include "Texture.h"
#include<d3d11.h>
#include <d3dx11.h>
#include<Windows.h>
class Texture2D :
	public Texture
{
private:
	ID3D11ShaderResourceView* m_RecourceView;
	ID3D11Device *m_pd3dDevice;
public:
	Texture2D(ID3D11Device *device);
	~Texture2D(void);

	bool loadFromFile(LPWCH,ID3D11DeviceContext* context);
	bool loadFromFile(char* filename,ID3D11DeviceContext* context);

	ID3D11ShaderResourceView* getTexture();
	void release();
	bool isReleased();
	void useTextureAt(ID3D11DeviceContext*,int);
};

