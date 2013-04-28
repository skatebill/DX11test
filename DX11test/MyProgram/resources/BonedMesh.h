#pragma once
#include "ModelMesh.h"
#define BONE_MAX_NUM 40
#define BONE_SLOT 1
struct boneConstantBuffer{
	XMMATRIX* boneTrans;
};
class BonedMesh:public ModelMesh
{
private:
	int* m_BoneId;
	int m_BoneNum;
public:
	BonedMesh(ID3D11Device*);
	virtual ~BonedMesh(void);

	void setBoneList(int* idlist,int num)
	{
		m_BoneId = idlist;
		m_BoneNum=num;
	}
	int* getBoneList(){
		return m_BoneId;
	}
	int getBoneNum(){
		return m_BoneNum;
	}
};

