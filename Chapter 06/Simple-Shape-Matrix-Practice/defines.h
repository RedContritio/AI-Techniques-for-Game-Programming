#ifndef __DEF__DEF
#define __DEF__DEF

#define __WINDOW__WIDTH 640
#define __WINDOW__HEIGHT 480

#define __FRAME__PER__SECOND 60

#define PI 3.141592653589793238462643383279

#define __MAX__ROTATION__PER__TICK 0.05
#define __MAX__SCALE__PER__TICK 0.1
#define __MAX__TRANSLATION__PER__TICK 1

//macros for the user input
#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

#endif