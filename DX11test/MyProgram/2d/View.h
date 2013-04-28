#pragma once
#include "2DBaseObject.h"
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

	void draw(ID3D11DeviceContext* context);
	void release(){m_QuadModel->release();m_QuadModel=0;};
	bool isReleased(){return m_QuadModel==0;}

};

