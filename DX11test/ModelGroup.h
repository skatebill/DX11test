#pragma once
#include "MyResource.h"
#include "ModelMesh.h"
#include "ShaderProgram.h"
#include <vector>
class ModelGroup :
	public MyResource
{
private:	
	std::vector<ModelMesh*> m_MeshList;
	ShaderProgram* m_Shader;
protected:
	ID3D11Device* m_Device;

public:
	ModelGroup(ID3D11Device* device);
	~ModelGroup(void);

	virtual void release();
	virtual bool isReleased();

	void addMesh(ModelMesh*);
	void useShader(ShaderProgram*);
	void updateConstantbuffer(ID3D11DeviceContext* pContext,int slot,void* buffer);
	virtual void draw(ID3D11DeviceContext* pContext);
};

