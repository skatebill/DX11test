#include "ObjectConrtoller.h"

ObjectController::ObjectController(ID3D11Device* device):MyObject(),m_device(device),m_staticId(0)
{

}

ObjectController::~ObjectController()
{
	Release();
}

void ObjectController::Release(){
	for(std::map<int,MyObject*>::iterator ite = m_objectList.begin();ite!= m_objectList.end();ite++)
	{
		(*ite).second->Release();
		m_objectList.erase(ite);
	}
}
bool ObjectController::isReleased(){
	return m_objectList.size()==0;
}
void ObjectController::visite(void* contextPtr){
	for(std::map<int,MyObject*>::iterator ite = m_objectList.begin();ite!= m_objectList.end();ite++)
	{
		(*ite).second->visit(contextPtr);
	}

}
