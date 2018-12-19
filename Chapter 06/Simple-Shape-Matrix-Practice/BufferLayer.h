#ifndef __BUFFER__LAYER
#define __BUFFER__LAYER
#include <windows.h>
namespace RedContritio
{
	class BUFFERLAYER
	{
		private :
			HDC hBufferLayer ;
			HBITMAP hBitmap ;
			HBITMAP hPrevBitmap ;
			void DeleteAll(void );
			void BuildAll(HWND );
		public :
			BUFFERLAYER(void );
			BUFFERLAYER(HWND );
			HDC GetHdc(void );
			void AdjustSize(HWND );
			void ResetBufferLayer(HWND );
			void DeleteBufferLayer(void );
			~BUFFERLAYER(void );
	};
}
#endif
