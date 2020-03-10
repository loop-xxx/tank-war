#include "RMiss2.h"
#include "Engine.h"


RMiss2::RMiss2()
	:RBase(0x4)
{

}

bool RMiss2::run (Map *map, Player *play)
{
	static clock_t start_time = 0;
	clock_t current_time = clock();
	if (current_time - start_time < 60)
		return false;
	start_time = current_time;
	int d1, d2;
	switch (m_dir)
	{
	case UP:
	case DOWN:
		d1 = LEFT;
		d2 = RIGHT;
		break;
	case LEFT :
	case RIGHT :
		 d1 = UP;
		 d2 = DOWN;

	}
	Missile *tmp;
	if (tmp = g_Game->m_mf.buyMissile(PID, d1, *this, map,m_color, 8, false))
		g_Game->m_MISSlist.push_back(tmp);
	if (tmp = g_Game->m_mf.buyMissile(PID, d2, *this, map, m_color, 8, false))
		g_Game->m_MISSlist.push_back(tmp);
	return moveMissile(map);
}

RMiss2::~RMiss2()
{
}
