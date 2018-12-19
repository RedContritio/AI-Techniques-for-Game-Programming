#ifndef __BUFFER__LAYER
#define __BUFFER__LAYER
#include <windows.h>
#pragma warning( disable: 4996)
namespace RedContritio
{
	class BUFFERLAYER
	{
		private :
			HDC hBufferLayer ;
			HBITMAP hBitmap ;
			HBITMAP hPrevBitmap ;
			void DeleteAll(void );
			void BuildAll(HWND hwnd );
		public :
			BUFFERLAYER(void );
			BUFFERLAYER(HWND hwnd );
			HDC GetHdc(void );
			void AdjustSize(HWND hwnd );
			void ResetBufferLayer(HWND hwnd );
			void DeleteBufferLayer();
			~BUFFERLAYER(void );
	};
}
#endif
