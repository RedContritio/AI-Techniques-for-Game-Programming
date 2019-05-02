#include "Mine.h"



Mine::Mine(void)
{
}


Mine::~Mine(void)
{
}


RedContritio::Vector2d Mine::Position(void) const
{
	return m_position;
}

void Mine::Reset(void)
{
	m_position = GetRandomVector2d();
}
