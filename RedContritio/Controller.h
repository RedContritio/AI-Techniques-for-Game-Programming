#ifndef CONTROLLER__INC
#define CONTROLLER__INC
#pragma once
#include <windows.h>
#include <queue>

namespace RedContritio{
class Controller
{
public :
	Controller(const int &cxClient ,const int &cyClient );
	~Controller(void );
	bool Update(void );
	void PushKey(int );
	void Render(HDC surface );
protected :
	void *m_pEntity ;
	const int &g_cxClient ;
	const int &g_cyClient ;
	HPEN *m_PenSolid ;
	unsigned int *m_RGBs ,m_NumOfRGBs ;
	HBRUSH *m_BrushSolid ,m_BrushTransparent ;
	std::queue<int> m_keys ;
	bool (*m_EntityUpdate)(void* ,unsigned );
	void (*m_EntityRender)(void* ,HDC );
	void CreatePaintTools(const unsigned int *RGBs ,int NumOfElements );
	void DeletePaintTools(void );
	bool (*SelectEntityUpdate(bool (*method )(void* ,unsigned )))(void* ,unsigned );
	bool (*ReleaseEntityUpdate(void ))(void* ,unsigned );
	void (*SelectEntityRender(void (*method )(void* ,HDC )))(void* ,HDC );
	void (*ReleaseEntityRender(void ))(void* ,HDC );
};
}
#endif