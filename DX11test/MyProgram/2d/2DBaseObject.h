#include <d3dx11.h>
#include <xnamath.h>
#include "..\resources\ModelGroup.h"
#include "..\resources\Effect.h"

namespace myprogram{
	struct ViewProMatrix{		
		XMMATRIX view;
		XMMATRIX projection;
	};
	class _2DBaseObject:public myprogram::effectedObject{
	private:
		float _x,_y;
		float _w,_h;
	public:
		static ViewProMatrix s_Matrix;

		_2DBaseObject():effectedObject(){}
		virtual ~_2DBaseObject(){}
		static void setBorder(int width,int height);

		void setPos(int x,int y){_x=x;_y=y;}
		void setSize(int w,int h){_w=w;_h=h;}
		
	};

}