#pragma once
#define RELEASE(x) if(x){x->Release();x=0;}




namespace myprogram{

	enum TYPE_ENUM
	{
	TYPE_UNKNOWN=100,
	TYPE_VIEW,
	TYPE_IMAGEVIEW
	};

	class MyObject
	{
	protected:
	public:
		int m_ObjectId;
		 TYPE_ENUM m_Type;

		MyObject(void)
		{
			m_Type=TYPE_UNKNOWN;
			m_ObjectId=0;
		}

		virtual ~MyObject(void)
		{
		}
		virtual void Release()=0;
		virtual bool isReleased()=0;
		virtual void visit(void* contextPtr){

		}

	};
}

