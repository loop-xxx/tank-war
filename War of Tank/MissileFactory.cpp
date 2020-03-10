#include "Missile.h"


unsigned int Missile::MissileFactory::MISSID = 0x66;

//初始化子弹池
Missile::MissileFactory::MissileFactory()
{
	for (int i = 0; i < MMIN; i++)
	{
		m_missilePool.push_back(new Missile(MISSID++));
	}
}

//购买普通子弹
Missile *Missile::MissileFactory::buyMissile(int tankID, int dir, COORD &pos, Map *map, unsigned int color)
{
	int x = pos.X;
	int y = pos.Y;
	switch (dir)
	{
	case UP:    y--; break;
	case DOWN:  y++; break;
	case LEFT:  x--; break;
	case RIGHT: x++; break;
	}
	
	Missile* tmp;
	if (m_missilePool.empty())
	{
		tmp = new Missile(MISSID++);
		if (!tmp->init(tankID, dir, x, y, color))
		{
			tmp = nullptr;
			delete tmp;
		}
		tmp->m_max = MAX;
		return tmp;
	}
	tmp = m_missilePool.back();
	if (!(tmp->init(tankID, dir, x, y, color)))
	{
		tmp->m_isValid = false;
		return nullptr;
	}
	tmp->m_max = MAX;
	m_missilePool.pop_back();
	
	return tmp;
}

//购买特效子弹
Missile *Missile::MissileFactory::buyMissile(int tankID, int dir, COORD &pos, Map *map, unsigned int color, int max, int isfly)
{
	int x = pos.X;
	int y = pos.Y;

	switch (dir)
	{
	case UP:    y--; break;
	case DOWN:  y++; break;
	case LEFT:  x--; break;
	case RIGHT: x++; break;
	}

	Missile* tmp;
	if (m_missilePool.empty())
	{
		tmp = new Missile(MISSID++);
		if (!(tmp->init(tankID, dir, x, y, color, max, isfly)))
		{
			tmp = nullptr;
			delete tmp;
		}
		return tmp;
	}
	tmp = m_missilePool.back();
	if (!(tmp->init(tankID, dir, x, y, color, max, isfly)))
	{
		tmp->m_isValid = false;
		return nullptr;
	}
	tmp->m_max = max;
	m_missilePool.pop_back();

	return tmp;
}

//购买反弹特效子弹
Missile *Missile::MissileFactory::buyMissile(int tankID, int dir, COORD &pos, Map *map, unsigned int color, int max, int isfly, int flag)
{
	int x = pos.X;
	int y = pos.Y;

	switch (dir)
	{
	case UP:    y--; break;
	case DOWN:  y++; break;
	case LEFT:  x--; break;
	case RIGHT: x++; break;
	}

	Missile* tmp;
	if (m_missilePool.empty())
	{
		tmp = new Missile(MISSID++);
		if (!(tmp->init(tankID, dir, x, y, color, max, isfly, flag)))
		{
			tmp = nullptr;
			delete tmp;
		}
		return tmp;
	}
	tmp = m_missilePool.back();
	if (!(tmp->init(tankID, dir, x, y, color,max, isfly, flag)))
	{
		tmp->m_isValid = false;
		return nullptr;
	}
	tmp->m_max = max;
	m_missilePool.pop_back();

	return tmp;
}


//回收子弹
void Missile::MissileFactory::recMissile(Missile *miss)
{
	miss->m_isValid = false;
	m_missilePool.push_back(miss);
}

//释放子弹池
void Missile::MissileFactory::clearPool()
{
	for (int i = 0; i < m_missilePool.size(); i++)
		delete m_missilePool[i];
	m_missilePool.clear();
}

Missile::MissileFactory::~MissileFactory()
{
	clearPool();
}

