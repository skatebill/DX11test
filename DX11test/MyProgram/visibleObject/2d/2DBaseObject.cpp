#include"2DBaseObject.h"

myprogram::ViewProMatrix myprogram::_2DBaseObject::s_Matrix;
myprogram::ViewProMatrix myprogram::_2DBaseObject::s_AntiMatrix;
MatrixStack* myprogram::_2DBaseObject::s_2Dstack=0;
void myprogram::_2DBaseObject::setBorder(int width,int height){
	if(!s_2Dstack)
		s_2Dstack=new MatrixStack();
	s_2Dstack->setCamera2D(width,height);
	s_2Dstack->Ortho(width,height);
}

