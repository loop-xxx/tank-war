#include "Tank.h"

//先初始化几辆坦克到缓存池
Tank::TankFactory::TankFactory()
{
	for (int i = 0; i < MIN; i++)
	{
		tankPool.push_back(new Tank(TANKID++));
	}
}

//制造坦克
Tank *Tank::TankFactory::buyTank(Map *map)
{
	Tank* tmp;

	if (tankPool.empty())
	{
		tmp =  new Tank(TANKID++);
	}
	else
	{
		tmp = tankPool.back();
		tankPool.pop_back();
	}
	tmp->init(map, m_blood);

	return tmp;
}

//回收已经死亡的坦克到坦克池
void Tank :: TankFactory::recTank(Tank *tank)
{
	tankPool.push_back(tank);
} 

//升级坦克
void Tank ::TankFactory:: updateTank(int levl)
{
	m_blood = DEFBLOOD + levl/4;
}

//释放坦克池
void Tank ::TankFactory::clearPool()
{
	for (int i = 0; i < tankPool.size(); i++)
		delete tankPool[i];
	tankPool.clear();
}

Tank :: TankFactory::~TankFactory()
{
	clearPool();
}

unsigned int Tank :: TankFactory:: TANKID = 0x10;
