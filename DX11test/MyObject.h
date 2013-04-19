#pragma once
#define RELEASE(x) if(x){x->Release();x=0;}
namespace myprogram{
	class MyObject
	{
	public:

		MyObject(void)
		{
		}

		virtual ~MyObject(void)
		{
		}
		virtual void release()=0;
		virtual bool isReleased()=0;
	};
}

