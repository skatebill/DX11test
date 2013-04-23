#include "BonedMeshGroup.h"


BonedMeshGroup::BonedMeshGroup(ID3D11Device* device):ModelGroup(device)
{
	m_CurFrame=0;
}


BonedMeshGroup::~BonedMeshGroup(void)
{

}

void BonedMeshGroup::addBonedMesh(BonedMesh* m){
	m_BonedMeshList.push_back(m);
}
void BonedMeshGroup::draw(ID3D11DeviceContext* context){
	ModelGroup::draw(context);
	
	for(std::vector<BonedMesh*>::iterator ite=m_BonedMeshList.begin();ite!=m_BonedMeshList.end();ite++)
	{
		boneConstantBuffer* buf=getBoneConstantBuffer((**ite).getBoneList(),(**ite).getBoneNum());
		updateConstantbuffer(context,1,buf->boneTrans);
		(**ite).draw(context);
		delete(buf->boneTrans);
		delete(buf);
	}
	
}
boneConstantBuffer* BonedMeshGroup::getBoneConstantBuffer(int* ids,int num){
	boneConstantBuffer* result=new boneConstantBuffer;
	XMMATRIX* m=(XMMATRIX*)malloc(40*64);;
	result->boneTrans=m;
	XMMATRIX rmatrix=XMMatrixIdentity();
	rmatrix=XMMatrixTranspose(rmatrix);
	for(int i=0;i<40;i++){
		memcpy(&m[i],&rmatrix,64);

	}
	m_CurFrame+=0.0333f;
	if(m_CurFrame>100)
	{
		m_CurFrame=0;
	}
	for(int i=0;i<num;i++)
	{
		boneSample sample=m_bgroup->getBoneSample(ids[i],(int)m_CurFrame);
		boneTM first=m_bgroup->getBone(ids[i])->getFirstTM();
		XMMATRIX rmatrix=XMMatrixTranslation(-first.translate.x,-first.translate.y,-first.translate.z);
		XMVECTOR t=XMVectorSet(first.rotate.x,first.rotate.y,first.rotate.z,0);
		t=XMVector3Normalize(t);
		rmatrix=rmatrix*XMMatrixRotationAxis(t,first.rotate.w );
		t=XMVectorSet(sample.tm.rotate.x,sample.tm.rotate.y,sample.tm.rotate.z,0);
		t=XMVector3Normalize(t);
		rmatrix=rmatrix*XMMatrixRotationAxis(t,-sample.tm.rotate.w  );
		rmatrix=rmatrix*XMMatrixTranslation(sample.tm.translate.x,sample.tm.translate.y,sample.tm.translate.z);
		rmatrix=XMMatrixTranspose(rmatrix);
		memcpy(&m[i],&rmatrix,64);
	}	
	
	return result;
}