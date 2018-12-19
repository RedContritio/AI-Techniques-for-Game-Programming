#ifndef __DEF__DEF
#define __DEF__DEF

#define FRAMES__PER__SECOND 60

#define	PI 3.14159265358979

#define SCALING_FACTOR 60

#define SHIP__MASS 100 // kg

#define THRUSH__PER__SECOND 350.0 // F (N)
#define ROTATION__PER__SECOND 3.0 // s^(-1)

#define __GRAVITY__MOON -1.63
#define __LANDER__SCALE 10.0

#define GRAVITY __GRAVITY__MOON

#define __TOLERANCE__DIST 20
#define __TOLERANCE__SPEED 0.5
#define __TOLERANCE__ANGLE PI/16

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

#endif