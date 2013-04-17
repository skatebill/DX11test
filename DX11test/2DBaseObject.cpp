#include"2DBaseObject.h"

myprogram::ViewProMatrix* myprogram::_2DBaseObject::s_Matrix=0;

void myprogram::_2DBaseObject::setBorder(int width,int height){
	if(s_Matrix)
		s_Matrix=new ViewProMatrix;
		
	XMVECTOR Eye = XMVectorSet( width/2,width/2, -5.0f, 0.0f );
	XMVECTOR At = XMVectorSet( width/2, width/2, 0.0f, 0.0f );
	XMVECTOR Up = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	s_Matrix->view = XMMatrixLookAtLH( Eye, At, Up );
		
	s_Matrix->projection = XMMatrixOrthographicLH(width,height,0,100);
}

