#include"MatrixStack.h"

MatrixStack::MatrixStack():m_CurTop(0){


	m_World=XMMatrixIdentity();
	m_View=XMMatrixIdentity();
	m_Projection=XMMatrixIdentity();
	m_atrixBuffer=(char*)malloc(MAX_MATRIX_DEPTH*sizeof(XMMATRIX));
}
MatrixStack::~MatrixStack(){
	Release();
}
void MatrixStack::Ortho(float width,float height,float znear,float zfar){
	m_Projection=XMMatrixOrthographicLH(width,height,znear,zfar);
}
void MatrixStack::Perspective(float angle,float rate,float znear,float zfar){
	m_Projection=XMMatrixPerspectiveFovLH( angle, rate, znear, zfar );
}
void MatrixStack::setCamera2D(int w,int h)
{
	XMVECTOR Eye = XMVectorSet(w/2.0f,- h/2.0f, -5.0f, 0.0f );
	XMVECTOR At = XMVectorSet(w/2.0f,- h/2.0f, 0.0f, 0.0f );
	XMVECTOR Up = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	m_View = XMMatrixLookAtLH( Eye, At, Up );
}
void MatrixStack::translate(float x,float y,float z){
	m_World*=XMMatrixTranslation(x,y,z);
}
void MatrixStack::translate2d(float x,float y){
	m_World*=XMMatrixTranslation(x,-y,0);
}
void MatrixStack::sclae(float x,float y,float z){
	m_World*=XMMatrixScaling(x,y,z);
}
void MatrixStack::rotate(float angle,float x,float y,float z){
	m_World*=XMMatrixRotationAxis(XMVectorSet(x,y,z,0),angle);
}

void MatrixStack::push(){
	if(m_CurTop>=99)
	{
		return;
	}
	memcpy(m_atrixBuffer+m_CurTop*sizeof(XMMATRIX),&m_World,sizeof(XMMATRIX));
	m_CurTop++;
}
void MatrixStack::pop(){
	if(m_CurTop==0)
	{
		return;
	}
	m_CurTop--;
	memcpy(&m_World,m_atrixBuffer+m_CurTop*sizeof(XMMATRIX),sizeof(XMMATRIX));
}
void MatrixStack::getBuffer(mvpCostantBuffer* buffer)
{
	buffer->World=XMMatrixTranspose(m_World);
	buffer->View=XMMatrixTranspose(m_View);
	buffer->Projection=XMMatrixTranspose(m_Projection);
}