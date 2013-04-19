#pragma once

#include <windows.h>
#include <list>
#include <map>
#include <stdio.h>
using namespace std;

LRESULT CALLBACK    MainWndProc( HWND, UINT, WPARAM, LPARAM );
/****************************************************************************
*	made by Ф��
*	�����Ĵ��ڹ�����
*	���𴴽����ڲ�������Ϣѭ��
*	�����Ĵ���Ĭ��Ϊ���ɸı��С�����������С��
*	��ʱֻ�ܴ���һ�����ڣ��������ʵ����ά����HWND
*	֧����֡�ʣ����ö�ȡcpu tick��Ŀ��ʽʵ��
*
*******************************************************************************/
class WindowController
{
private:
	int m_Width;
	int m_Height;
	
	float m_fps;
	float m_dstFps;

	double mCPUFreq;
	bool isFPSLocked;
	LARGE_INTEGER   PT_litmp;
	double m_LastTime;

	static list<WindowController*> *s_windowList;
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

	virtual void render(float delta)=0;
	virtual void intiData()=0;
	virtual void cleanup()=0;
	
	virtual void keyDown(int);
	virtual void keyUp(int);
	virtual void MouseDown(int);
	virtual void MouseUp(int);
	virtual void MouseMove(int);

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
	float getFPS(){return m_fps;}
	void setFPS(float dstFPS){m_dstFps=dstFPS;}
	void lockFPS(){isFPSLocked=true;}
	void unlockFPS(){isFPSLocked=false;}
	void updateFps();
	virtual LRESULT CALLBACK    WndProc( HWND, UINT, WPARAM, LPARAM );
	static list<WindowController*>* getAllWindow(){
		int i=s_windowList->size();
		if(i<1) return 0;
		return s_windowList;}
	
};