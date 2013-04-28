#pragma once
#include"..\MyObject.h"
#include <vector>
namespace myprogram{
	class effectBase:public myprogram::MyObject{
	private:
		int m_Type;
	public:
		effectBase():m_Type(0){}
		virtual ~effectBase(){}
		virtual void update(float)=0;
		virtual void use()=0;
	};

	class effectedObject:public myprogram::MyObject{
	protected:
		std::vector<effectBase*> m_effectList;
	public:
		effectedObject(){}
		virtual ~effectedObject(){}

		void addEffect(effectBase* eff){m_effectList.push_back(eff);}
		void removeEffect(){m_effectList.clear();}

	};
}