#include "WindowController.h"


WindowController::WindowController(HINSTANCE instance)
{
	m_hInst=instance;
}


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
	m_Height=w;

    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof( WNDCLASSEX );
    wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
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
    return S_OK;
}
int WindowController::run()
{
	
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
			this->render();
		}

    }
	return msg.wParam;
}
//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch( message )
    {
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
