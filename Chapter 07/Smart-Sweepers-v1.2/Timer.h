#ifndef TIMER__RC__INC
#define TIMER__RC__INC
#include <windows.h>

namespace RedContritio
{
	class Timer
	{
		private :
			ULONGLONG currentTime ;
			ULONGLONG lastTime ;
			ULONGLONG nextTime ;
			ULONGLONG frameTime ;
			ULONGLONG perFrameCountFreq ;
			double timeElapsed ;
			double timeScale ;
			double fps ;
		public :
			Timer(void );
			Timer(double fps );
			void Start(void );
			bool ReadyForNextFrame(void );
			double GetTimeElapsed(void );
			double TimeElapsed(void );
	};
}
#endif
