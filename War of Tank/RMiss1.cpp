#include "RMiss1.h"
#include "Engine.h"

#define STEP_MAX 2
RMiss1::RMiss1()
	:RBase(0x0), m_step(STEP_MAX)
{
}

bool RMiss1::init(unsigned int tankId, int dir, int x, int y, unsigned int color)
{
	m_tmpX = x;
	m_tmpY = y;
	return Missile::init(tankId, dir, x, y, color);
}
#define STEP_TIME 200
bool RMiss1::run (Map *map, Player *play)
{
	static clock_t start_time = 0;
	clock_t current_time;
	if (m_isValid)
	{
		start_time = clock();

		int d1, d2;
		switch (m_dir)
		{
		case UP:
		case DOWN:
			d1 = LEFT;
			d2 = RIGHT;
			break;
		case LEFT:
		case RIGHT:
			d1 = UP;
			d2 = DOWN;
		}
		Missile *tmp;
		if (tmp = g_Game->m_mf.buyMissile(PID, d1, *this, map, m_color, 1, false))
			g_Game->m_MISSlist.push_back(tmp);
		if (tmp = g_Game->m_mf.buyMissile(PID, d2, *this, map, m_color, 1, false))
			g_Game->m_MISSlist.push_back(tmp);
		m_isValid = !moveMissile(map);
		return false;
	}
	else
	{
		if (m_step)
		{
			current_time = clock();
			if (current_time - start_time < STEP_TIME)
				return false;
			m_step--;
			start_time = current_time;
			this->init(m_tankID, m_dir, m_tmpX, m_tmpY, F_H_GREEN);
		}
		else
		{
			return true;
		}
	}
	return false;
}

bool RMiss1::checkMissile()
{
	int y = Y;
	int x = X;

	switch (m_dir)
	{
	case UP:    y--; break;
	case DOWN:  y++; break;
	case LEFT:  x--; break;
	case RIGHT: x++;  break;
	}

	int tmp;
	if ((tmp = (*g_Game->m_map)[y][x]) == WALL_D || tmp == WALL_A)
	{
		return false;
	}
	else if (tmp == WALL_C)
	{
		hitWall(y, x);
	}
	else if (tmp != WALL__ && tmp != WALL_E)
	{
		hitWall(y, x);
	}
	else if (g_Game->m_map->isPlaced(x, y))
	{
		if (g_Game->m_map->getRecID(x, y) <0x60)
		{
			hitTank(x, y);
		}
		else
		{
			hitMissile(x, y);
		}
	}
	
	return true;
}


RMiss1::~RMiss1()
{
}
