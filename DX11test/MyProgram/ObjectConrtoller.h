#pragma once

#include"MyObject.h"
#include<map>
#include <D3D11.h>
class ObjectController:public myprogram::MyObject{
private:
	std::map<int,MyObject*> m_objectList;
	ID3D11Device* m_device;
	int m_staticId;
public:
	ObjectController(ID3D11Device* device);
	~ObjectController();

	int addObject(MyObject* obj){
		m_objectList[m_staticId]=obj;
		return m_staticId++;
	}

	void Release();
	bool isReleased();
	void visite(void* contextPtr);
};