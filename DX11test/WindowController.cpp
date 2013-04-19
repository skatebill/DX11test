#include "WindowController.h"


WindowController::WindowController(HINSTANCE instance):m_dstFps(60.0f),isFPSLocked(true),m_LastTime(0)
{
	m_hInst=instance;
	
	QueryPerformanceFrequency(&PT_litmp);
	mCPUFreq =(double)PT_litmp.QuadPart;
}

list<WindowController*>* WindowController::s_windowList = new list<WindowController*>();


WindowController::~WindowController(void)
{
}


HRESULT WindowController::createWindow(LPCWSTR title,int w,int h,bool fullscreen)
{
	if(!m_hInst)
	{
		return E_FAIL;
	}

	m_Width=w;
	m_Height=h;

    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof( WNDCLASSEX );
    wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = MainWndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = m_hInst;
    wcex.hIcon = LoadIcon( m_hInst, ( LPCTSTR )107 );
    wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
    wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"MyWindowClass";
    wcex.hIconSm = LoadIcon( wcex.hInstance, ( LPCTSTR )107 );
    if( !RegisterClassEx( &wcex ) )
        return E_FAIL;
	DWORD styles = WS_OVERLAPPEDWINDOW & ~(WS_SIZEBOX |WS_MAXIMIZEBOX|WS_MINIMIZEBOX) ;
    RECT rc = { 0, 0, m_Width, m_Height };
    AdjustWindowRect( &rc, styles , FALSE);

	int screenW=GetSystemMetrics(SM_CXSCREEN);
	int screenH=GetSystemMetrics(SM_CYSCREEN);
	int newposx= (screenW>>1)-((rc.right - rc.left)>>1);
	int newposy= (screenH>>1)-((rc.bottom - rc.top)>>1);

    m_hWnd = CreateWindow( L"MyWindowClass", title, styles,
                           newposx, newposy, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, m_hInst,
                           NULL );
    if( !m_hWnd )
        return E_FAIL;

    ShowWindow( m_hWnd, SW_SHOW );
	s_windowList->push_back(this);
    return S_OK;
}
int WindowController::run()
{
	intiData();

    MSG msg = {0};
    while( WM_QUIT != msg.message )
    {
        if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
		else
		{
			QueryPerformanceCounter(&PT_litmp);   
			double tick = PT_litmp.QuadPart;                 //获得起始的值
			if(!m_LastTime)
			{
				m_LastTime=tick;
			}
			double deltatick=tick-m_LastTime;
			float deltatime = deltatick / mCPUFreq;

			m_fps = 1 / deltatime;

			static float duration=1;
			duration+=deltatime;
			if(duration>1.0f)
			{
				duration=0;
				updateFps();
			}

			this->render(deltatime);
			
			m_LastTime=tick;
			if(isFPSLocked)
			{
				double lockTick;
				while(deltatime < 1  /m_dstFps){
					QueryPerformanceCounter(&PT_litmp);   
					lockTick = PT_litmp.QuadPart;                 //获得起始的值   

					deltatick=lockTick-m_LastTime;
					deltatime = deltatick / mCPUFreq;

				}
			}
		}

    }
	cleanup();
	return msg.wParam;
}
void WindowController::updateFps(){
	static bool first=true;
	static WCHAR buf[32];
	static WCHAR dst[32];
	if(first){
		first=false;
		GetWindowText(m_hWnd,buf,32);
	}
	swprintf_s(dst,L"%s fps : %f",buf,m_fps);
	SetWindowText(m_hWnd,dst);
}
//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WindowController::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch( message )
    {
	case WM_KEYDOWN:
		keyDown(wParam);
		break;
	case WM_KEYUP:
		keyUp(wParam);
		break;
	case WM_PAINT:
		hdc = BeginPaint( hWnd, &ps );
		EndPaint( hWnd, &ps );
		break;

	case WM_DESTROY:
		PostQuitMessage( 0 );
		break;

	default:
		return DefWindowProc( hWnd, message, wParam, lParam );
    }

    return 0;
}


void WindowController::keyDown(int){

}
void WindowController::keyUp(int keyCode){
	if(keyCode == VK_ESCAPE)
	{
		PostQuitMessage(0);
	}

}
void WindowController::MouseDown(int){

}
void WindowController::MouseUp(int){

}
void WindowController::MouseMove(int){

}




LRESULT CALLBACK    MainWndProc(  HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	
	list<WindowController*>* window=WindowController::getAllWindow();
	if(window)
	{
		list<WindowController*>::iterator iter=window->begin();
		do{
			if(*iter != 0 &&(*iter)->getHwnd() == hWnd)
			{
				return (*iter)->WndProc(hWnd,message,wParam,lParam);
			}
			iter++;
		}while(iter++ != WindowController::getAllWindow()->end());
	}

    return DefWindowProc( hWnd, message, wParam, lParam );
}