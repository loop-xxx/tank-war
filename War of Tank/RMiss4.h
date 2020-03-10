#pragma once
#include "RBase.h"
class RMiss4 :
	public RBase
{
private :
	int m_step;
public:
	RMiss4();
	bool run(Map *map, Player *play);
	bool init(unsigned int tankId, int dir, int x, int y, unsigned int color);
	~RMiss4();
};

