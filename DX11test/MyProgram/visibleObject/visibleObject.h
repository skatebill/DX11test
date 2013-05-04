#pragma once
#include"../MyObject.h"
#include"../MatrixStack.h"

namespace myprogram{
	class VisibleObject:public myprogram::MyObject{
	private:
		bool m_visible;
	public:
		virtual void draw(ID3D11DeviceContext* context,MatrixStack* stack)=0;
		virtual void update(float d){}

		void setVisible(bool visible){m_visible=visible;}
		bool isVisible(){return m_visible;}
		virtual void preRender(){

		}
		virtual void postRender(){

		}
	};
}