#pragma once

#include <windows.h>

LRESULT CALLBACK    WndProc( HWND, UINT, WPARAM, LPARAM );

class WindowController
{
protected:
	HINSTANCE               m_hInst;
	HWND                    m_hWnd;
public:
	WindowController(HINSTANCE instance);
	virtual ~WindowController(void);

	virtual HRESULT createWindow(LPCWSTR title,int w=800,int h=600,bool fullscreen=false);
	HINSTANCE getInstance(){return m_hInst;}
	HWND getHwnd(){return m_hWnd;}
	int run();
	virtual void render()=0;

};

