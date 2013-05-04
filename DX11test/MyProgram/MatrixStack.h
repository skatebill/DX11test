#pragma once
#include<D3D11.h>
#include<xnamath.h>
#include"MyObject.h"
#define MAX_MATRIX_DEPTH 100
struct mvpCostantBuffer{
	XMMATRIX	World;
	XMMATRIX	View;
	XMMATRIX	Projection;
};
class MatrixStack:public myprogram::MyObject{
private:	
	char* m_atrixBuffer;
	int m_CurTop;
public:
	XMMATRIX	m_World;
	XMMATRIX	m_View;
	XMMATRIX	m_Projection;


	MatrixStack();
	~MatrixStack();

	void Ortho(float width,float height,float znear=0.01f,float zfar=1000.0f);
	void Perspective(float angle,float rate,float znear=0.01f,float zfar=1000.0f);

	void setCamera2D(int w,int h);

	void getBuffer(mvpCostantBuffer* buffer);

	void translate(float x,float y,float z);
	void translate2d(float x,float y);
	void sclae(float x,float y,float z);
	void rotate(float angle,float x,float y,float z);

	void Release(){free(m_atrixBuffer);m_atrixBuffer=0;}
	bool isReleased(){return m_atrixBuffer==0;}

	void push();
	void pop();
};
