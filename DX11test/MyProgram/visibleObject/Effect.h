#pragma once
#include"visibleObject.h"
#include <vector>
namespace myprogram{
	class effectBase:public myprogram::MyObject{
	public:
		effectBase(){}
		virtual ~effectBase(){}
		virtual void update(float)=0;
		virtual void use()=0;
	};

	class effectedObject:public myprogram::VisibleObject{
	protected:
		std::vector<effectBase*> m_effectList;
	public:
		effectedObject(){}
		virtual ~effectedObject(){}

		void addEffect(effectBase* eff){m_effectList.push_back(eff);}
		void removeEffect(){m_effectList.clear();}

	};
}