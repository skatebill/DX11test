#pragma once
#include "modelgroup.h"
#include "BonedMesh.h"
#include "BoneGroup.h"
class BonedMeshGroup :
	public ModelGroup
{
private:	
	std::vector<BonedMesh*> m_BonedMeshList;
	BoneGroup* m_bgroup;
	float m_CurFrame;
public:
	BonedMeshGroup(ID3D11Device*);
	~BonedMeshGroup(void);

	virtual void Release(){}
	virtual bool isReleased(){return false;}

	void addBonedMesh(BonedMesh* m);
	void setBone(BoneGroup* boneG){m_bgroup=boneG;}
	virtual void draw(ID3D11DeviceContext* pContext);
	boneConstantBuffer* getBoneConstantBuffer(int* ids,int num);
};

