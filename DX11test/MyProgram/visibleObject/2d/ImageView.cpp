#include"ImageView.h"


ImageView::ImageView(ID3D11Device* device):View(device),m_Image(0){
	m_Image=new TexturedRenderable(device);
	m_Type=TYPE_IMAGEVIEW;
}

ImageView::~ImageView(){
	Release();
}

void ImageView::draw(ID3D11DeviceContext* context,MatrixStack* stack)
{
	if(m_Image)
		m_Image->draw(context,stack);
}