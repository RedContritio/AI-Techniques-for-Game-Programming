#ifndef __UTILS__HPP__INC
#define __UTILS__HPP__INC

template<typename T >
void Clamp(T& arg ,T min ,T max )
{
	if(arg < min )arg = min ;
	if(arg > max )arg = max ;
}

#endif