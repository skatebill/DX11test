#pragma once
#include"MyResource.h"
#include<d3d11.h>
#include<D3DX11.h>
#include<vector>
#include <xnamath.h>
struct VertexPU{
	XMFLOAT3 pos;
	XMFLOAT2 tex;
};
class Model:public MyResource
{
private:
	ID3D11Buffer* m_VertexBuffer;
	ID3D11Buffer* m_IndexBuffer;

	ID3D11Device* m_Device;
	int m_IndexNum;
	int m_NumVertex;
	UINT m_Stride;

public:
	Model(ID3D11Device*);
	~Model(void);

	void release();
	bool isReleased();

	bool setVertexSource(void*,int,int);
	bool setIndexSource(void*,int);

	ID3D11Buffer* getVertexBuffer();
	ID3D11Buffer* getIndexBuffer();

	void useModel(ID3D11DeviceContext* context){
		UINT offset=0;
		context->IASetVertexBuffers( 0, 1, &m_VertexBuffer, &m_Stride, &offset );
		// Set index buffer
		context->IASetIndexBuffer( m_IndexBuffer, DXGI_FORMAT_R16_UINT, 0 );};

	int getIndexNum(){
		return m_IndexNum;
	}
};



Model* getQuadModel(ID3D11Device* ,int w,int h);