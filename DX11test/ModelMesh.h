#pragma once
#include"MyResource.h"
#include<d3d11.h>
#include<D3DX11.h>
#include<vector>
#include <xnamath.h>
#include "Texture2D.h"
struct VertexPU{
	XMFLOAT3 pos;
	XMFLOAT2 tex;
};
struct VertexPNU{
	XMFLOAT3 pos;
	XMFLOAT3 normal;
	XMFLOAT2 tex;
};
class ModelMesh:public MyResource
{
private:
	ID3D11Buffer* m_VertexBuffer;
	ID3D11Buffer* m_IndexBuffer;

	ID3D11Device* m_Device;
	int m_IndexNum;
	int m_NumVertex;
	UINT m_Stride;

	Texture2D *m_Texture;
	int m_DefualtSlot;

public:
	ModelMesh(ID3D11Device*);
	~ModelMesh(void);

	void release();
	bool isReleased();

	bool setVertexSource(void*,int,int);
	bool setIndexSource(void*,int);
	bool loadTexture(char*);
	void setTextureSlot(int slot){m_DefualtSlot=slot;}

	ID3D11Buffer* getVertexBuffer();
	ID3D11Buffer* getIndexBuffer();

	void useModel(ID3D11DeviceContext* context){
		UINT offset=0;
		context->IASetVertexBuffers( 0, 1, &m_VertexBuffer, &m_Stride, &offset );
		// Set index buffer
		context->IASetIndexBuffer( m_IndexBuffer, DXGI_FORMAT_R16_UINT, 0 );
	}
	
	void useModel(ID3D11DeviceContext* context,int slot){
		UINT offset=0;
		context->IASetVertexBuffers( 0, 1, &m_VertexBuffer, &m_Stride, &offset );
		// Set index buffer
		context->IASetIndexBuffer( m_IndexBuffer, DXGI_FORMAT_R16_UINT, 0 );
		useTextureAt(context,slot);
	}
	int getIndexNum(){
		return m_IndexNum;
	}
	void draw(ID3D11DeviceContext* context,int slot=-1)
	{
		if(slot==-1)
		{
			slot = m_DefualtSlot;
		}
		useModel(context,slot);
		context->DrawIndexed(m_IndexNum,0,0);
	}
	void useTextureAt(ID3D11DeviceContext* context,int slot){
		if(m_Texture){
			m_Texture->useTextureAt(context,slot);
		}
	}
};



ModelMesh* getQuadModel(ID3D11Device* ,int w,int h);