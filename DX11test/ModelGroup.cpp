#include "ModelGroup.h"


ModelGroup::ModelGroup(ID3D11Device* device):m_Device(0),m_Shader(0)
{
	m_Device= device;
}


ModelGroup::~ModelGroup(void)
{
	release();
}

void ModelGroup::release(){
	for(std::vector<ModelMesh*>::iterator ite=m_MeshList.begin();ite!=m_MeshList.end();ite++)
	{
		(**ite).release();
	}
	m_MeshList.clear();
}

bool ModelGroup::isReleased(){
	return m_MeshList.size() == 0;
}



void ModelGroup::addMesh(ModelMesh* mesh){
	m_MeshList.push_back(mesh);
}
void ModelGroup::useShader(ShaderProgram* shader){
	m_Shader=shader;
}

void ModelGroup::updateConstantbuffer(ID3D11DeviceContext* context,int slot,void* buffer)
{
	if(m_Shader)
	{
		m_Shader->setConstantBuffer(context,slot,buffer);
	}

}
void ModelGroup::draw(ID3D11DeviceContext* context){
	if(m_Shader)
	{
		m_Shader->use(context);
	}
	for(std::vector<ModelMesh*>::iterator ite=m_MeshList.begin();ite!=m_MeshList.end();ite++)
	{

		(**ite).draw(context);
	}
}