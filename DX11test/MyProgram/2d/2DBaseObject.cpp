#include"2DBaseObject.h"

myprogram::ViewProMatrix myprogram::_2DBaseObject::s_Matrix;

void myprogram::_2DBaseObject::setBorder(int width,int height){
		
	XMVECTOR Eye = XMVectorSet(width/2.0f,- height/2.0f, -5.0f, 0.0f );
	XMVECTOR At = XMVectorSet(width/2.0f,- height/2.0f, 0.0f, 0.0f );
	XMVECTOR Up = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	XMMATRIX view = XMMatrixLookAtLH( Eye, At, Up );
	XMMATRIX test;
	test=view;
	s_Matrix.view = view;
		
	s_Matrix.projection = XMMatrixOrthographicLH(width,height,0,100);
}

