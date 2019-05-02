#ifndef _MINE_INC
#define _MINE_INC
#pragma once

#include "Vector2d.h"

class Mine
{
private:
	RedContritio::Vector2d m_position;
public:
	Mine(void);
	~Mine(void);
	RedContritio::Vector2d Position(void) const;
	void Reset(void);
};

#endif