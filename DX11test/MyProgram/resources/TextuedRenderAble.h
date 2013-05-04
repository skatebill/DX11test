#pragma once
#include"RenderAble.h"
#include"Texture2D.h"


class TexturedRenderable:public RenderAble2d{
private:
	Texture2D* m_Tex;

public:
	TexturedRenderable(ID3D11Device* device):RenderAble2d(device),m_Tex(0){}
	virtual ~TexturedRenderable(){}

	void setTexture(Texture2D* tex){m_Tex=tex;}
	void loadTexture(char* textureName,ID3D11DeviceContext* context){
		if(!m_Tex)
		{
			m_Tex=new Texture2D(m_Device);
		}else
		{
			m_Tex->Release();
		}
		m_Tex->loadFromFile(textureName,context);
	}
	virtual void preRender(ID3D11DeviceContext* context,MatrixStack* stack){
		RenderAble2d::preRender(context,stack);
		if(m_Tex)
		{
			m_Tex->useTextureAt(context,0);
		}
	}
	virtual void Release(){RELEASE(m_QuadModel);RELEASE(m_shader);}
};