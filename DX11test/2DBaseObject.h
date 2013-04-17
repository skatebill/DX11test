#include"MyObject.h"
#include <d3dx11.h>
#include <xnamath.h>

namespace myprogram{
	struct ViewProMatrix{		
		XMMATRIX view;
		XMMATRIX projection;
	};
	class _2DBaseObject:public myprogram::MyObject{
	private:
		float _x,_y;
		static ViewProMatrix* s_Matrix;
	public:
		_2DBaseObject():MyObject(){}
		virtual ~_2DBaseObject(){}
		static void setBorder(int width,int height);
	};

}