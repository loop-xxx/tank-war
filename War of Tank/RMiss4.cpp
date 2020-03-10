#include "RMiss4.h"
#include "Engine.h"

#define STEP_MAX 14
RMiss4::RMiss4()
	:RBase(0x4), m_step(STEP_MAX)
{
}
bool RMiss4::init(unsigned int tankId, int dir, int x, int y, unsigned int color)
{

	switch (dir)
	{
	case UP:    y++; break;
	case DOWN:  y--; break;
	case LEFT:  x++; break;
	case RIGHT: x--; break;
	}
	Y = y;
	X = x;
	m_color = color;

	return true;
}

bool RMiss4::run(Map *map, Player *play)
{
	static clock_t start_time = 0;
	clock_t current_time = clock();
	if (current_time - start_time < 200)
		return false;
	start_time = current_time;
	Missile *tmp;
	if (m_step >= 10)
	{
		if (m_step == STEP_MAX)
			showMessage("Õ¨µ¯°²×°¡£¡£¡£",m_color,0);

		writeChar(Y, X, "¡Ñ", F_H_GREEN);
		m_step--;
		return false;
	}
	else
	{
		if (m_step)
		{
			writeChar(Y, X, blood_pic[m_step], m_color);
			for (int i = LEFT; i <= DOWN; i++)
			{
				if (tmp = g_Game->m_mf.buyMissile(PID, i, *this, g_Game->m_map, m_color, 6, true))
					g_Game->m_MISSlist.push_back(tmp);
				if (tmp = g_Game->m_mf.buyMissile(PID, i, *this, g_Game->m_map, m_color, 6, false))
					g_Game->m_MISSlist.push_back(tmp);
			}
			m_step--;
			return false;
		}
		else
		{
			int t = (*map)[Y][X];
			writeChar(Y, X, wall_pic[t], wall_color[t]);
			g_Game->m_player->DrawTank(g_Game->m_map);
			return true;
		}
	}
	
}

RMiss4::~RMiss4()
{
}
