#pragma once
#include "MyResource.h"
#include "ModelMesh.h"
#include "ShaderProgram.h"
#include <vector>
class ModelGroup :
	public MyResource
{
private:	
	ID3D11Device* m_Device;
	std::vector<ModelMesh*> m_MeshList;
	ShaderProgram* m_Shader;
public:
	ModelGroup(ID3D11Device* device);
	~ModelGroup(void);

	void release();
	bool isReleased();

	void addMesh(ModelMesh*);
	void useShader(ShaderProgram*);
	void updateConstantbuffer(ID3D11DeviceContext* pContext,int slot,void* buffer);
	void draw(ID3D11DeviceContext* pContext);
};

