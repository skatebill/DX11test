//--------------------------------------------------------------------------------------
// File: Tutorial01.cpp
//
// This application demonstrates creating a Direct3D 11 device
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"d3dx11.lib")
//--------------------------------------------------------------------------------------
// File: Tutorial04.cpp
//
// This application displays a 3D cube using Direct3D 11
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include"DX11WBASE.h"
#include "ShaderProgram.h"
#include <xnamath.h>
#include "Texture2D.h"
#include "Sampler.h"
#include "ModelMesh.h"
#include "View.h"
#include "phrase\Chunk.h"





mainChunk* modelChunk;























struct SimpleVertex
{
    XMFLOAT3 Pos;
    XMFLOAT4 Color;
    XMFLOAT2 texCoord;
};


struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
};
Texture2D* tex;
Sampler* samp;
ModelMesh* model;
View* view;




class test:public DX11WBASE{
private:
	ShaderProgram *m_shader;

	XMMATRIX           m_World;
	XMMATRIX           m_View;
	XMMATRIX           m_Projection;

	ID3D11Buffer*           m_pVertexBuffer;
	ID3D11Buffer*           m_pIndexBuffer;
	ID3D11Buffer*           m_pConstantBuffer;

	int numTodraw;
public:
	test(HINSTANCE instance):DX11WBASE(instance),m_pVertexBuffer(0),m_pIndexBuffer(0),m_pConstantBuffer(0){};
	~test(){};
	void render(float delta){
		float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red,green,blue,alpha
		m_pImmediateContext->ClearRenderTargetView( m_pRenderTargetView, ClearColor );
		static float t=0;
		t+=delta;

		//
		// Animate the cube
		//
		m_World = XMMatrixRotationY( t );


		//
		// Update variables
		//
		ConstantBuffer cb;
		cb.mWorld = XMMatrixTranspose( m_World );
		cb.mView = XMMatrixTranspose( m_View );
		cb.mProjection = XMMatrixTranspose( m_Projection);
		m_pImmediateContext->UpdateSubresource( m_pConstantBuffer, 0, NULL, &cb, 0, 0 );
		//m_shader->setConstantBuffer(m_pImmediateContext,0,&cb);

		//
		// Renders a triangle
		//
		m_pImmediateContext->VSSetShader( m_shader->getVertexShader(), NULL, 0 );
		m_pImmediateContext->VSSetConstantBuffers( 0, 1, &m_pConstantBuffer );
		m_pImmediateContext->PSSetShader( m_shader->getPixelShader(), NULL, 0 );
		m_pImmediateContext->DrawIndexed( numTodraw, 0, 0 );        // 36 vertices needed for 12 triangles in a triangle list

		//view->draw(m_pImmediateContext);
		m_pSwapChain->Present(0,0);
	}
	void intiData(){
		

		chunkReader reader;
		modelChunk = reader.readFile("456.BINARY");
		meshGroupChunk* mesh=(meshGroupChunk*)modelChunk->subchunk[0];
		int num = mesh->getSubCount();
		int numvertex=0;
		for(int i=0;i<num;i++)
		{
			numvertex+=((meshChunk*)mesh->subchunk[i])->indexlist.size()*3;
		}

		ModelMesh* people=new ModelMesh(m_pd3dDevice);

		SimpleVertex *data=new SimpleVertex[numvertex];
		WORD*	index=new WORD[numvertex];
		for(int i=0;i<numvertex;i++)
		{
			index[i]=i;

		}
		int curVertex=0;
		for(int i=0;i<num;i++)
		{
			meshChunk* meshc=((meshChunk*)mesh->subchunk[i]);
			int meshVertexNum=meshc->indexlist.size();
			for(int vi=0;vi<meshVertexNum;vi++)
			{
				data[curVertex].Pos=XMFLOAT3(meshc->vertexlist[meshc->indexlist[vi]->a]->pos.x,meshc->vertexlist[meshc->indexlist[vi]->a]->pos.y,meshc->vertexlist[meshc->indexlist[vi]->a]->pos.z);
				curVertex++;
				data[curVertex].Pos=XMFLOAT3(meshc->vertexlist[meshc->indexlist[vi]->b]->pos.x,meshc->vertexlist[meshc->indexlist[vi]->b]->pos.y,meshc->vertexlist[meshc->indexlist[vi]->b]->pos.z);
				curVertex++;
				data[curVertex].Pos=XMFLOAT3(meshc->vertexlist[meshc->indexlist[vi]->c]->pos.x,meshc->vertexlist[meshc->indexlist[vi]->c]->pos.y,meshc->vertexlist[meshc->indexlist[vi]->c]->pos.z);
				curVertex++;

			}

		}
		numTodraw=numvertex;

		m_shader=new ShaderProgram(m_pd3dDevice);
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		UINT numElements = ARRAYSIZE( layout );
		m_shader->setLayout(layout,numElements);
		m_shader->requestConstantBuffer(192);
		HRESULT hr=m_shader->loadShader(L"Tutorial04.fx");
		if(FAILED(hr))
		{
			exit(0);
		}
		m_shader->useLayout(m_pImmediateContext);

		
		SimpleVertex vertices[] =
		{
			{ XMFLOAT3( -1.0f, 1.0f, -1.0f ), XMFLOAT4( 0.0f, 0.0f, 1.0f, 1.0f ) ,XMFLOAT2(0,0)},
			{ XMFLOAT3( 1.0f, 1.0f, -1.0f ), XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f ) ,XMFLOAT2(0,2) },
			{ XMFLOAT3( 1.0f, 1.0f, 1.0f ), XMFLOAT4( 0.0f, 1.0f, 1.0f, 1.0f ) ,XMFLOAT2(2,0) },
			{ XMFLOAT3( -1.0f, 1.0f, 1.0f ), XMFLOAT4( 1.0f, 0.0f, 0.0f, 1.0f ) ,XMFLOAT2(2,2) },
			{ XMFLOAT3( -1.0f, -1.0f, -1.0f ), XMFLOAT4( 1.0f, 0.0f, 1.0f, 1.0f ) ,XMFLOAT2(2,0) },
			{ XMFLOAT3( 1.0f, -1.0f, -1.0f ), XMFLOAT4( 1.0f, 1.0f, 0.0f, 1.0f ) ,XMFLOAT2(0,2) },
			{ XMFLOAT3( 1.0f, -1.0f, 1.0f ), XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f ) ,XMFLOAT2(0,0) },
			{ XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f ) ,XMFLOAT2(0,2) },
		};

		D3D11_BUFFER_DESC bd;
		ZeroMemory( &bd, sizeof(bd) );
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof( SimpleVertex ) * numvertex;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory( &InitData, sizeof(InitData) );
		InitData.pSysMem = data;	
		hr = m_pd3dDevice->CreateBuffer( &bd, &InitData, &m_pVertexBuffer );
		if( FAILED( hr ) )
			exit(0);
		D3D11_RASTERIZER_DESC rasterDesc;

		rasterDesc.AntialiasedLineEnable = false;  
		rasterDesc.CullMode = D3D11_CULL_BACK;  
		rasterDesc.DepthBias = 0;  
		rasterDesc.DepthBiasClamp = 0.0f;  
		rasterDesc.DepthClipEnable = true;  
		rasterDesc.FillMode = D3D11_FILL_WIREFRAME;  
		rasterDesc.FrontCounterClockwise = false;  
		rasterDesc.MultisampleEnable = false;  
		rasterDesc.ScissorEnable = false;  
		rasterDesc.SlopeScaledDepthBias = 0.0f;  
		ID3D11RasterizerState* state;
		m_pd3dDevice->CreateRasterizerState(&rasterDesc,&state);
		m_pImmediateContext->RSSetState(state);

		unlockFPS();
		// Create index buffer
		WORD indices[] =
		{
			3,1,0,
			2,1,3,

			0,5,4,
			1,5,0,

			3,4,7,
			0,4,3,

			1,6,5,
			2,6,1,

			2,7,6,
			3,7,2,

			6,4,5,
			7,4,6,
		};
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof( WORD ) * numvertex;        // 36 vertices needed for 12 triangles in a triangle list
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		InitData.pSysMem = index;
		hr = m_pd3dDevice->CreateBuffer( &bd, &InitData, &m_pIndexBuffer );
		if( FAILED( hr ) )
			exit(0);
		

		/*model=new Model(m_pd3dDevice);
		model->setVertexSource(vertices,sizeof(SimpleVertex),8);
		model->setIndexSource(indices,36);
		
		model->useModel(m_pImmediateContext);*/
		getQuadModel(m_pd3dDevice,1,1)->useModel(m_pImmediateContext);
		// Set vertex buffer
		UINT stride = sizeof( SimpleVertex );
		UINT offset = 0;
		m_pImmediateContext->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );
		// Set index buffer
		m_pImmediateContext->IASetIndexBuffer( m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0 );

		// Set primitive topology
		m_pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

		// Create the constant buffer
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(ConstantBuffer);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		hr = m_pd3dDevice->CreateBuffer( &bd, NULL, &m_pConstantBuffer );
		if( FAILED( hr ) )
			exit(0);

		// Initialize the world matrix
		m_World = XMMatrixIdentity();

		// Initialize the view matrix
		XMVECTOR Eye = XMVectorSet( 0.0f, 1.0f, -5.0f, 0.0f );
		XMVECTOR At = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
		XMVECTOR Up = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
		m_View = XMMatrixLookAtLH( Eye, At, Up );

		// Initialize the projection matrix
		m_Projection = XMMatrixPerspectiveFovLH( XM_PIDIV2, getWindowWidth() / (FLOAT)getWindowHeight(), 0.01f, 100.0f );
		
		tex=new Texture2D(m_pd3dDevice);
		tex->loadFromFile(L"seafloor.dds");
		tex->useTextureAt(m_pImmediateContext,0);
		samp=new Sampler(m_pd3dDevice);
		samp->createSampleState(D3D11_FILTER_MIN_MAG_MIP_LINEAR,D3D11_TEXTURE_ADDRESS_WRAP);
		samp->useSamplerAt(m_pImmediateContext,0);
		if( FAILED( hr ) )
			exit(0);

		View::setBorder(getWindowWidth(),getWindowHeight());
		view=new View(m_pd3dDevice);



	}
	void cleanup(){

	}
};

//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	test t(hInstance);
	t.createWindow(L"test");
	t.run();

    return 1;
}

