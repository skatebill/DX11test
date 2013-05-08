#pragma once
#include"View.h"
#include"..\..\resources\TextuedRenderAble.h"
#include"..\..\..\winbase\dx11\ShaderProgram.h"
class ImageView:public View{
private:
	TexturedRenderable* m_Image;
public:
	ImageView(ID3D11Device*);
	virtual ~ImageView();
	
	void setTexture(Texture2D* tex){
		if(m_Image)
			m_Image->setTexture(tex);
	}
	void setTexture(ID3D11ShaderResourceView* tex){
		if(m_Image)
			m_Image->setTexture(tex);
	}
	void loadTexture(char* textureName,ID3D11DeviceContext* context){
		if(m_Image)
		{
			m_Image->loadTexture(textureName,context);
		}
	}
	void setShader(ShaderProgram* shader){
		if(m_Image)
			m_Image->setShader(shader);
	}
	void setSize(int w,int h){
		View::setSize(w,h);
		if(m_Image)
			m_Image->setSize(w,h);
	}
	void setPos(int x,int y){
		View::setPos(x,y);
		if(m_Image)
			m_Image->setPos(x,y);
	}
	void draw(ID3D11DeviceContext* context,MatrixStack* stack);

	void Release(){View::Release();RELEASE(m_Image);}
	bool isReleased(){return View::isReleased();}

};