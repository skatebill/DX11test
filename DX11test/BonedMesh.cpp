#include "BonedMesh.h"


BonedMesh::BonedMesh(ID3D11Device* device):ModelMesh(device)
{
	
}


BonedMesh::~BonedMesh(void)
{
	if(m_BoneId)
		delete m_BoneId;
}
