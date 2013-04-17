#pragma once
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

