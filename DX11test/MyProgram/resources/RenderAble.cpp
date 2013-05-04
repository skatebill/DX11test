#include"RenderAble.h"

void RenderAble2d::draw(ID3D11DeviceContext* context,MatrixStack* stack)
{
	preRender(context,stack);
	render(context,stack);
	postRender();
}


void RenderAble2d::preRender(ID3D11DeviceContext* context,MatrixStack* stack){
	if(m_shader)
	{
		m_shader->use(context);		
	}
}
void RenderAble2d::postRender(){

}
void RenderAble2d::render(ID3D11DeviceContext* context,MatrixStack* stack){	
	stack->push();
	stack->sclae(_w,_h,1);
	stack->translate2d(_x,_y);
	m_QuadModel->useModel(context);	
	mvpCostantBuffer buffer;
	stack->getBuffer(&buffer);
	m_shader->setConstantBuffer(context,0,&buffer);
	context->DrawIndexed(m_QuadModel->getIndexNum(),0,0);
	stack->pop();
	
}