#include"2DBaseObject.h"

myprogram::ViewProMatrix myprogram::_2DBaseObject::s_Matrix;
myprogram::ViewProMatrix myprogram::_2DBaseObject::s_AntiMatrix;

void myprogram::_2DBaseObject::setBorder(int width,int height){
		
	XMVECTOR Eye = XMVectorSet(width/2.0f,- height/2.0f, -5.0f, 0.0f );
	XMVECTOR At = XMVectorSet(width/2.0f,- height/2.0f, 0.0f, 0.0f );
	XMVECTOR Up = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	XMMATRIX view = XMMatrixLookAtLH( Eye, At, Up );

	s_Matrix.view =XMMatrixTranspose( view);
	s_Matrix.projection =XMMatrixTranspose(  XMMatrixOrthographicLH(width,height,0,100));
	XMVECTOR t;
	
	s_AntiMatrix.view = XMMatrixTranspose(XMMatrixInverse(&t,view));
	s_AntiMatrix.projection = XMMatrixTranspose(XMMatrixInverse(&t,XMMatrixOrthographicLH(width,height,0,100)));
}

