#include <d3dx11.h>
#include <xnamath.h>
#include "..\..\resources\ModelGroup.h"
#include "..\Effect.h"
#include "..\..\..\winbase\dx11\ShaderProgram.h"

namespace myprogram{
	struct ViewProMatrix{		
		XMMATRIX view;
		XMMATRIX projection;
	};
	class _2DBaseObject:public myprogram::effectedObject{
	private:
	protected:
		float _x,_y;
		float _w,_h;
		ShaderProgram* m_shader;
	public:
		static MatrixStack* s_2Dstack;
		static ViewProMatrix s_Matrix;
		static ViewProMatrix s_AntiMatrix;

		_2DBaseObject():m_shader(0),effectedObject(){
			_x=0;
			_y=0;
			_w=0;
			_h=0;
		}
		virtual ~_2DBaseObject(){}
		static void setBorder(int width,int height);

		void setPos(int x,int y){_x=x;_y=y;}
		void setSize(int w,int h){_w=w;_h=h;}

		void setShader(ShaderProgram* shader){m_shader=shader;}
		void visit(void* contextPtr){
			preRender();
			draw((ID3D11DeviceContext*)contextPtr,s_2Dstack);
			postRender();
		}
		
	};

}