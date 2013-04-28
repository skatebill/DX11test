
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"d3dx11.lib")
#include"winbase\dx11\DX11WBASE.h"
#include "winbase\dx11\ShaderProgram.h"
#include "MyProgram\resources\ModelGroup.h"
#include "MyProgram\resources\ModelPhraser.h"
#include "winbase\dx11\shaderResources.h"
#include "MyProgram\resources\Sampler.h"
#include "lua\MyLuaManager.h"
#include "lua\Lua_Function.h"

struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
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
			float ClearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f }; // red,green,blue,alpha
			m_pImmediateContext->ClearRenderTargetView( m_pRenderTargetView, ClearColor );
			m_pImmediateContext->ClearDepthStencilView(m_depthStencilView,D3D11_CLEAR_DEPTH,1.0,0);
			static float t=0;
			t+=delta;
			static float pos=5;
			static float direction=-1;
			pos+=direction*delta;
			if(pos<0.2)
			{
				pos=0.2;
				direction=1;
			}
			if(pos>5)
			{
				pos=5;
				direction=-1;

			}
		//	m_World = XMMatrixRotationY(t);
			ConstantBuffer cb;
			cb.mWorld = XMMatrixTranspose( m_World );
			cb.mView = XMMatrixTranspose( m_View );
			cb.mProjection = XMMatrixTranspose( m_Projection);
			people->updateConstantbuffer(m_pImmediateContext,0,&cb);

			people->draw(m_pImmediateContext);
			
			MyLuaManager::getInstance()->doFile("lua/lua_script/render.lua");

			m_pSwapChain->Present(0,0);
		}
		
	void intiData(){
		people = loadPUBModel("resources/456.BINARY",m_pd3dDevice,m_pImmediateContext);

		
		// Initialize the world matrix
		m_World = XMMatrixIdentity();

		// Initialize the view matrix
		XMVECTOR Eye = XMVectorSet( 0, 1.1, -2.0, 0.0f );
		XMVECTOR At = XMVectorSet( 0.0f, 1.1, 0.0f, 0.0f );
		XMVECTOR Up = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
		m_View = XMMatrixLookAtLH( Eye, At, Up );

		// Initialize the projection matrix
		m_Projection = XMMatrixPerspectiveFovLH( XM_PIDIV4, getWindowWidth() / (FLOAT)getWindowHeight(), 0.01f, 1000.0f );
		m_Projection = XMMatrixOrthographicLH(4,4*getWindowHeight()/(FLOAT)getWindowWidth(),0,100);
		
		bone_shader =new ShaderProgram(m_pd3dDevice);
		bone_shader->setLayout(CUSTOM_LAYOUT_PUB,CUSTOM_LAYOUT_PUB_NUM);
		bone_shader->requestConstantBuffer(192,0);
		bone_shader->requestConstantBuffer(sizeof(XMMATRIX)*BONE_MAX_NUM,1);
		bone_shader->loadShader(L"FX/boneAnime.fx");
		people->useShader(bone_shader);



		Sampler* samp=new Sampler(m_pd3dDevice);
		samp->createSampleState(D3D11_FILTER_MIN_MAG_MIP_LINEAR,D3D11_TEXTURE_ADDRESS_WRAP,-1);
		samp->useSamplerAt(m_pImmediateContext,0);

		MyLuaManager::getInstance()->registerFun(ConsleGlue);

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

