
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"d3dx11.lib")
#include"DX11WBASE.h"
#include "ShaderProgram.h"
#include "ModelGroup.h"
#include "ModelPhraser.h"
#include "shaderResources.h"
#include "Sampler.h"

struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
};
struct BCONS{
	
};
struct vertexB{
	XMFLOAT3 pos;
	XMFLOAT2 tex;
	float bone[40];
	vertexB(){
		ZeroMemory(bone,sizeof(float)*40);
	}
};


class test:public DX11WBASE{
private:
	ModelGroup* people;
	ShaderProgram* shader;

	ShaderProgram* bone_shader;
	ModelMesh* boneTest;
	ModelGroup* bone;

	
	XMMATRIX           m_World;
	XMMATRIX           m_View;
	XMMATRIX           m_Projection;
public:
	test(HINSTANCE instance):DX11WBASE(instance){};
	~test(){};
		void render(float delta){
		float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red,green,blue,alpha
		m_pImmediateContext->ClearRenderTargetView( m_pRenderTargetView, ClearColor );
		m_pImmediateContext->ClearDepthStencilView(m_depthStencilView,D3D11_CLEAR_DEPTH,1.0,0);
		static float t=0;
		t+=delta;
//		m_World = XMMatrixRotationY(t);
		ConstantBuffer cb;
		cb.mWorld = XMMatrixTranspose( m_World );
		cb.mView = XMMatrixTranspose( m_View );
		cb.mProjection = XMMatrixTranspose( m_Projection);
		bone_shader->setConstantBuffer(m_pImmediateContext,0,&cb);

		people->draw(m_pImmediateContext);
		//bone->draw(m_pImmediateContext);
		m_pSwapChain->Present(0,0);
		}
		
	void intiData(){
		people = loadPUBModel("456.BINARY",m_pd3dDevice);
		shader = new ShaderProgram(m_pd3dDevice);
		shader->setLayout(CUSTOM_LAYOUT_PU,CUSTOM_LAYOUT_PU_NUM);
		shader->requestConstantBuffer(192);
		shader->loadShader(L"Tutorial03.fx");
		//shader->useLayout(m_pImmediateContext);

		
		// Initialize the world matrix
		m_World = XMMatrixIdentity();

		// Initialize the view matrix
		XMVECTOR Eye = XMVectorSet( 0, 1, -2.0, 0.0f );
		XMVECTOR At = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
		XMVECTOR Up = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
		m_View = XMMatrixLookAtLH( Eye, At, Up );

		// Initialize the projection matrix
		m_Projection = XMMatrixPerspectiveFovLH( XM_PIDIV2, getWindowWidth() / (FLOAT)getWindowHeight(), 0.01f, 1000.0f );

		
		bone_shader =new ShaderProgram(m_pd3dDevice);
		bone_shader->setLayout(CUSTOM_LAYOUT_PUB,CUSTOM_LAYOUT_PUB_NUM);
		bone_shader->requestConstantBuffer(192,0);
		bone_shader->requestConstantBuffer(sizeof(XMMATRIX)*BONE_MAX_NUM,1);
		bone_shader->loadShader(L"boneAnime.fx");
		people->useShader(bone_shader);
		

		vertexB test[3];
		test[0].pos=XMFLOAT3(-10,0,0);
		test[1].pos=XMFLOAT3(0,10,0);
		test[2].pos=XMFLOAT3(10,0,0);

		test[0].bone[1]=1;

		WORD testi[3]={0,1,2};

		boneTest=new ModelMesh(m_pd3dDevice);
		boneTest->setVertexSource(test,sizeof(vertexB),3);
		boneTest->setIndexSource(testi,3);
		bone=new ModelGroup(m_pd3dDevice);
		bone->useShader(bone_shader);
		bone->addMesh(boneTest);
		
		Sampler* samp=new Sampler(m_pd3dDevice);
		samp->createSampleState(D3D11_FILTER_MIN_MAG_MIP_LINEAR,D3D11_TEXTURE_ADDRESS_WRAP);
		samp->useSamplerAt(m_pImmediateContext,0);
		
	/*D3D11_DEPTH_STENCIL_DESC drawRefDesc;
	drawRefDesc.DepthEnable = true;
	drawRefDesc.DepthFunc = D3D11_COMPARISON_LESS;
	drawRefDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	drawRefDesc.StencilEnable = true;
	drawRefDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	drawRefDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	drawRefDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	drawRefDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	drawRefDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	drawRefDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	drawRefDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	drawRefDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	drawRefDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	drawRefDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;


	ID3D11DepthStencilState* stateOut=0;
	if(FAILED(m_pd3dDevice->CreateDepthStencilState(&drawRefDesc,&stateOut)))
	{
		MessageBox(NULL,L"Create 'DrawReflection' depth stencil state failed!",L"Error",MB_OK);
	}
	m_pImmediateContext->OMSetDepthStencilState(stateOut,0x1);*/
	setDrawMode(Triangle);		
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

