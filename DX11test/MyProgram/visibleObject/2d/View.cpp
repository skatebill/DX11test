#include "View.h"


View::View(ID3D11Device* device)
{
	m_QuadModel=getQuadModel(device,400,300);
	m_Type=TYPE_VIEW;
}


View::~View(void)
{

}

void View::draw(ID3D11DeviceContext* context,MatrixStack* stack){
	/*if(stack==0)
	{
		stack=s_2Dstack;
	}

	if(m_shader)
	{
		stack->push();
		m_shader->use(context);
		mvpCostantBuffer buffer;
		stack->getBuffer(&buffer);
		m_shader->setConstantBuffer(context,0,&buffer);
		m_QuadModel->useModel(context);	
		context->DrawIndexed(m_QuadModel->getIndexNum(),0,0);
		stack->pop();
	}else
	{
		m_QuadModel->useModel(context);	
		context->DrawIndexed(m_QuadModel->getIndexNum(),0,0);
	}*/
}
