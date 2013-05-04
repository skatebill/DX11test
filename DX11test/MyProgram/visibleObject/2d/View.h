#pragma once
#include "2DBaseObject.h"
#include"..\..\MatrixStack.h"
using namespace myprogram;

class View :
	public _2DBaseObject
{
private:
	ID3D11Device* m_Device;
	ModelMesh*	m_QuadModel;
public:
	View(ID3D11Device*);
	virtual ~View(void);

	void draw(ID3D11DeviceContext* context,MatrixStack* stack);
	void Release(){m_QuadModel->Release();m_QuadModel=0;};
	bool isReleased(){return m_QuadModel==0;}

};

