#pragma once
#include"ModelMesh.h"
#include"..\..\winbase\dx11\ShaderProgram.h"
#include"..\MatrixStack.h"

class RenderAble:public MyObject{	
protected:
	ModelMesh*	m_QuadModel;
	ShaderProgram* m_shader;
	ID3D11Device* m_Device;
public:
	RenderAble(ID3D11Device* device):MyObject(),m_shader(0),m_QuadModel(0){
		m_Device=device;
	}
	void setShader(ShaderProgram* shader){m_shader=shader;}
	virtual void Release(){RELEASE(m_QuadModel);RELEASE(m_shader);}
	virtual bool isReleased(){return m_QuadModel==0;}
	
	virtual void preRender(ID3D11DeviceContext* context,MatrixStack* stack)=0;
	virtual void postRender()=0;
	virtual void render(ID3D11DeviceContext* context,MatrixStack* stack)=0;

};
class RenderAble2d:public RenderAble{	
protected:
	float _w,_h;
	float _x,_y;
public:
	RenderAble2d(ID3D11Device* device):RenderAble(device),_x(0),_y(0),_w(0),_h(0){
		m_QuadModel=getQuadModel(device,1,1);
		m_Device=device;
	}
	virtual ~RenderAble2d(){
		Release();
	}

	virtual void draw(ID3D11DeviceContext* context,MatrixStack* stack);
	void setSize(int w,int h){_w=w;_h=h;}
	void setPos(int x,int y){_x=x;_y=y;}

	virtual void preRender(ID3D11DeviceContext* context,MatrixStack* stack);
	virtual void postRender();
	virtual void render(ID3D11DeviceContext* context,MatrixStack* stack);

};