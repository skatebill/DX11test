
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
#include "MyProgram\visibleObject\2d\ImageView.h"
#include "winbase\dx11\RenderTarget.h"
#include "MyProgram\ObjectConrtoller.h"
#include"MyProgram\MatrixStack.h"
#include"MyProgram\resources\TextuedRenderAble.h"
class test:public DX11WBASE{
private:
	
	ImageView* quard;
	ShaderProgram* view_shader;

	RenderTarget* backTarget;

	TexturedRenderable* testrenderable;
	ObjectController* controller;
public:
	test(HINSTANCE instance):DX11WBASE(instance){};
	~test(){};
		void render(float delta){
			float ClearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f }; // red,green,blue,alpha
			float ClearColor2[4] = { 0.1f, 0.2f, 0.3f, 1.0f }; // red,green,blue,alpha
			clearTargetColor(m_pRenderTargetView,ClearColor);
			clearTargetDepth(m_depthStencilView,1);



			
		
			controller->visite(m_pImmediateContext);
           
			presentDraw();
		}
		
	void intiData(){


		Sampler* samp=new Sampler(m_pd3dDevice);
		samp->createSampleState(D3D11_FILTER_MIN_MAG_MIP_LINEAR,D3D11_TEXTURE_ADDRESS_WRAP,-1);
		samp->useSamplerAt(m_pImmediateContext,0);





		/***********************************************************************/
		
		view_shader= new ShaderProgram(m_pd3dDevice);
		view_shader->setLayout(CUSTOM_LAYOUT_PU,CUSTOM_LAYOUT_PU_NUM);
		view_shader->requestConstantBuffer(64*3,0);
		view_shader->loadShader(L"FX/Tutorial04.fx");

		quard=new ImageView(m_pd3dDevice);
		quard->setBorder(getWindowWidth(),getWindowHeight());
		quard->setShader(view_shader);
		quard->loadTexture("3f56cda0.dds",m_pImmediateContext);
		quard->setSize(400,300);
		quard->setPos(0,100);

		backTarget=RenderTarget::generateRenderTarget(m_pd3dDevice,getWindowWidth(),getWindowHeight());

		/**********************************************************************/
		setDrawMode(Triangle);
		/*********************************************************************/
		controller=new ObjectController(m_pd3dDevice);
		controller->addObject(quard);

	/*	testrenderable=new TexturedRenderable(m_pd3dDevice);
		testrenderable->setShader(view_shader);
		testrenderable->setSize(400,300);
		testrenderable->loadTexture("3f56cda0.dds",m_pImmediateContext);*/
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