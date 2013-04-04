#pragma once

#include <windows.h>

LRESULT CALLBACK    WndProc( HWND, UINT, WPARAM, LPARAM );

class WindowController
{
private:
	int m_Width;
	int m_Height;
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
	
	int getWindowWidth(){return m_Width;}
	int getWindowHeight(){return m_Height;}
	void moveWindow(int x,int y){
		SetWindowPos(m_hWnd,HWND_TOP,x,y,0,0,SWP_NOSIZE);
	}
	void moveWindowOffset(int dx,int dy){
		RECT rc;
		GetWindowRect(m_hWnd,&rc);
		int newx=rc.left+dx;
		int newy=rc.top+dy;
		moveWindow(newx,newy);

	}

};