#ifndef __BUFFER__LAYER
#define __BUFFER__LAYER
#include <windows.h>
namespace RedContritio
{

struct BUFFERLAYER
{
	HDC hdc;
	HBITMAP hBitmap;
	HBITMAP hPrevBitmap;
	
	BUFFERLAYER(void);
	BUFFERLAYER(HWND);

	void release(void); // shouldn't be called
	void build(HWND); // shouldn't be called

	void Adjust(HWND); // adjust size to hwnd
	void Reset(HWND); // create a buffer layer compatible to hwnd
	void Delete(void); // delete the buffer layer
	~BUFFERLAYER(void);
};

}
#endif
