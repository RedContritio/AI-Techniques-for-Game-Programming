#include "Controller.h"
#include <cstdlib>
#include <cassert>
static const unsigned int Colors[] ={RGB(0x00 ,0x00 ,0x00 ),
									 RGB(0xFF ,0x00 ,0x00 ),RGB(0x00 ,0xFF ,0x00 ),RGB(0x00 ,0x00 ,0xFF ),
									 RGB(0xFF ,0xFF ,0x00 ),RGB(0xFF ,0x00 ,0xFF ),RGB(0x00 ,0xFF ,0xFF ),
									 RGB(0xFF ,0xFF ,0X00 )};
namespace RedContritio{

Controller::Controller(const int &cxClient ,const int &cyClient ):g_cxClient(cxClient ),g_cyClient(cyClient ),
m_pEntity(NULL ),m_EntityUpdate(NULL ),m_EntityRender(NULL )
{
	CreatePaintTools(Colors ,sizeof(Colors )/sizeof(Colors[0] ));
}

Controller::~Controller(void )
{
	DeletePaintTools();
}

bool Controller::Update(void)
{
	bool TrueFlag = true ;
	unsigned int arg_passin =0 ;
	while(m_keys.size() >0 )
	{
		switch(m_keys.front())
		{
			default :
			{
				break ;
			}
		}
		m_keys.pop();
	}
	if(m_EntityUpdate !=NULL &&m_pEntity !=NULL )
	{
		if(!m_EntityUpdate(m_pEntity ,arg_passin ))
		{
			;
		}
	}
	return true ;
}

void Controller::Render(HDC surface)
{
	
	;

	if(m_EntityRender !=NULL &&m_pEntity !=NULL )
	{
		m_EntityRender(m_pEntity ,surface );
	}
	return ;
}

void Controller::PushKey(int vk )
{
	m_keys.push(vk );
	return ;
}



#pragma warning(push)
#pragma warning(disable:4018)
void Controller::CreatePaintTools(const unsigned int *RGBs ,int NumOfElements )
{
	m_NumOfRGBs =NumOfElements ;
	m_PenSolid =(HPEN *)malloc(m_NumOfRGBs *sizeof(HPEN ));
	m_BrushSolid =(HBRUSH *)malloc(m_NumOfRGBs *sizeof(HBRUSH ));
	m_RGBs =(unsigned int *)malloc(m_NumOfRGBs *sizeof(unsigned int ));
	memmove(m_RGBs ,RGBs ,NumOfElements *sizeof(unsigned int ));
	assert(m_PenSolid &&m_BrushSolid );
	for(int i=0 ;i<m_NumOfRGBs ;i++ )
	{
		m_PenSolid[i] =(HPEN )CreatePen(PS_SOLID ,1 ,RGBs[i] );
		m_BrushSolid[i] =(HBRUSH )CreateSolidBrush(RGBs[i] );
	}
	m_BrushTransparent =(HBRUSH )GetStockObject(NULL_BRUSH );
	return ;
}

void Controller::DeletePaintTools(void )
{
	for(int i=0 ;i<m_NumOfRGBs ;i++ )
	{
		if(m_PenSolid[i] )DeleteObject(m_PenSolid[i] );
		if(m_BrushSolid[i] )DeleteObject(m_BrushSolid[i] );
	}
	if(m_PenSolid )
	{
		free(m_PenSolid );
		m_PenSolid =NULL ;
	}
	if(m_BrushSolid )
	{
		free(m_PenSolid );
		m_PenSolid =NULL ;
	}
	return ;
}

bool(*Controller::SelectEntityUpdate(bool(*method )(void* ,unsigned)))(void* ,unsigned)
{
	bool (*tmpMethod)(void* ,unsigned ) =m_EntityUpdate ;
	m_EntityUpdate =method  ;
	return tmpMethod ;
}

bool (*(Controller::ReleaseEntityUpdate(void )))(void* ,unsigned )
{
	return SelectEntityUpdate(NULL );
}

void(*Controller::SelectEntityRender(void (*method )(void* ,HDC)))(void* ,HDC)
{
	void (*tmpMethod)(void* ,HDC ) =m_EntityRender ;
	m_EntityRender =method  ;
	return tmpMethod ;
}

void(*Controller::ReleaseEntityRender(void))(void* ,HDC)
{
	return SelectEntityRender(NULL );
}
#pragma warning(pop)

}