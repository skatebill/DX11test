#include "View.h"


View::View(ID3D11Device* device)
{
	m_QuadModel=getQuadModel(device,400,300);
}


View::~View(void)
{

}

void View::draw(ID3D11DeviceContext* context){
	m_QuadModel->useModel(context);
	context->DrawIndexed(m_QuadModel->getIndexNum(),0,0);
}
