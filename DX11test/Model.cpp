#include "Model.h"


Model::Model(ID3D11Device* device):MyResource(),m_VertexBuffer(0),m_IndexBuffer(0),m_IndexNum(0)
{
	m_Device=device;
}


Model::~Model(void)
{
	release();
}

void Model::release()
{
	RELEASE(m_VertexBuffer);
	RELEASE(m_IndexBuffer);
}
bool Model::isReleased()
{
	return m_VertexBuffer == 0;
}

bool Model::setVertexSource(void* data,int datasize,int numvertex)
{	
	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof(bd) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = datasize * numvertex;
	m_Stride=datasize;
	m_NumVertex=numvertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory( &InitData, sizeof(InitData) );
	InitData.pSysMem = data;
	HRESULT hr = m_Device->CreateBuffer( &bd, &InitData, &m_VertexBuffer );
	if( FAILED( hr ) )
		return false;

	return true;
}
bool Model::setIndexSource(void* data,int indexNum)
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof(bd) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( WORD ) * indexNum;       
	m_IndexNum = indexNum;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory( &InitData, sizeof(InitData) );
	InitData.pSysMem = data;
	HRESULT hr = m_Device->CreateBuffer( &bd, &InitData, &m_IndexBuffer );
	if( FAILED( hr ) )
		return false;

	return true;
}



Model* getQuadModel(ID3D11Device* device,int w,int h){
	VertexPU vertex[]={
		{XMFLOAT3(0,0,0),XMFLOAT2(0,1)},
		{XMFLOAT3(w,0,0),XMFLOAT2(1,1)},
		{XMFLOAT3(w,-h,0),XMFLOAT2(1,0)},
		{XMFLOAT3(0,-h,0),XMFLOAT2(0,0)}};
	WORD	index[]={0,1,3,1,2,3};
	Model* result=new Model(device);
	result->setVertexSource(vertex,sizeof(VertexPU),4);
	result->setIndexSource(index,6);
	return result;

}