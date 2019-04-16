#include "CController.h"

const int NumStars = 40;

const int	 NumPadVerts = 4;
const SPoint PadShape[NumPadVerts] = {SPoint(-20 ,0 ),
                                 SPoint(20 ,0 ),
                                 SPoint(20 ,5 ),
                                 SPoint(-20 ,5 )};

CController::CController(void ):Succeeded(false ),PadPos(S2DVector(RandFloat() * cxClient ,50 ))
{
	S2DVector vStartPos( 0.5*cxClient, cyClient - 50.0);
	lander = new CLander(vStartPos ,PadPos ,PI );
	int i ;
	for(i=0 ;i<NumPadVerts ;i++ )
	{
		vecPadVB.push_back(PadShape[i] );
	}
	for(i=0 ;i<NumStars ;i++ )
	{
		vecStarsVB.push_back(SPoint(RandInt(1 ,cxClient-1 ) ,RandInt(100 ,cyClient )));
	}
}

CController::~CController(void )
{
	if(lander )
	{
		delete lander ;
	}
}

bool CController::Update(double TimeElapsed )
{
	lander->UpdateShip(TimeElapsed );
	return true ;
}

void CController::NewRun(void )
{
	PadPos = S2DVector(50.0 + RandFloat() * (cxClient-100.0) ,50.0);
	lander->Reset(PadPos );
	return ;
}

void CController::Render(HDC& hdc )
{
	SetMapMode(hdc ,MM_ANISOTROPIC );
	SetViewportExtEx(hdc ,1 ,-1 ,NULL );
	SetWindowExtEx(hdc ,1 ,1 ,NULL );
	SetViewportOrgEx(hdc ,0 ,cyClient ,NULL );
	// Set Mapping
	HPEN OldPen = (HPEN)SelectObject(hdc ,GetStockObject(WHITE_PEN) );
	int i ;//fuck C++
	for(i=0 ;i<NumStars ;i++ )
	{
		if(RandFloat() > 0.05 )
		{
			SetPixel(hdc , (int)(vecStarsVB[i].x), (int)(vecStarsVB[i].y), RGB(255 ,255 ,255 ));
		}
	}

	RenderLandingPad(hdc );
	lander->Render(hdc );

	SetMapMode(hdc ,MM_ANISOTROPIC );
	SetViewportExtEx(hdc ,1 ,1 ,NULL );
	SetWindowExtEx(hdc ,1 ,1 ,NULL );
	SetViewportOrgEx(hdc ,0 ,0 ,NULL );

	SetBkMode(hdc ,TRANSPARENT );
	SetTextColor(hdc ,RGB(0,0,255) );
	
	const char* InstructionText = "Cursor Keys - Rotate   Spacebar - Thrust   R - Retry";
	int IsTlen = strlen(InstructionText );
	TextOut(hdc ,(cxClient - 7*IsTlen )/2 ,cyClient -20 ,InstructionText ,IsTlen );
	SelectObject(hdc ,OldPen );
	return ;
}

void CController::Resize(void )
{
	vecStarsVB.clear();
	int i ;//fuck VC++
	for(i=0 ;i<NumStars ;i++ )
	{
		vecStarsVB.push_back(SPoint(RandInt(1 ,cxClient-1 ) ,RandInt(100 ,cyClient )));
	}
}

void CController::RenderLandingPad(HDC &hdc )
{
	vector<SPoint> vecRLPVB ;
	vecRLPVB = vecPadVB ;
	int i ;
	for(i=0 ;i<NumPadVerts ;i++ )
	{
		vecRLPVB[i].x += PadPos.x ;
		vecRLPVB[i].y += PadPos.y ;
	}
	MoveToEx(hdc, (int)(vecRLPVB[NumPadVerts-1].x), (int)(vecRLPVB[NumPadVerts-1].y), NULL);
	for(i=0 ;i<NumPadVerts ;i++ )
	{
		LineTo(hdc, (int)(vecRLPVB[i].x), (int)(vecRLPVB[i].y));
	}
	return ;
}