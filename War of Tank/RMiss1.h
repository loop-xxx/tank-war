#pragma once
#include "RBase.h"

class RMiss1:
	public RBase
{
private:
	int m_step;
	int m_tmpX;
	int m_tmpY;
public:
	bool run(Map *map, Player *play);
	bool init(unsigned int tankId, int dir, int x, int y, unsigned int color);
	RMiss1();
	bool checkMissile();
	~RMiss1();
};
