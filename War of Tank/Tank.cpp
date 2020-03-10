#include "Engine.h"
#include "Tank.h"


Tank::Tank(unsigned int id)
	:m_id(id), m_bloodMAX(DEFBLOOD), m_blood(m_bloodMAX), m_dir(UP), m_stat(die), m_score(DEFSCORE), m_color(F_H_GREEN)
{
}

Tank::Tank(unsigned int id, WORD color, int boolMAX, int score)
	: m_id(id), m_bloodMAX(boolMAX), m_blood(boolMAX), m_dir(UP), m_stat(die), m_score(score), m_color(color)
{
}


//在坦克地图上和控制台上画出坦克
void Tank:: DrawTank(Map *map)
{
	byte(*tankArr)[3] = tank_style[(m_blood > m_bloodMAX/3 ?0:1)][m_dir];
	int y, x;
	unsigned int color = m_color;
	if (m_stat == inv)
		color = F_H_RED;

	for (int row = -1, i = 0; row <= 1; row++, i++)
	{
		for (int col = -1, j = 0; col <= 1; col++, j++)
		{
			y = Y + row; //行坐标
			x = X + col; //列坐标

			int tmp = tankArr[i][j];
			if (tmp)
			{
				map->putRes(y, x, m_id);
				char * pic = tank_pic[tmp];
				writeChar(y, x, pic, color);

				//草地的显示等级比坦克高（地图元素封装成类后，可以直接查看其显示等级）
				if ((*map)[y][x] == WALL_A) //草地
				{
					writeChar(y, x, wall_pic[WALL_A], wall_color[WALL_A] | color);
				}
			}

		}
	}
	x = X;
	y = Y;
	//添加血量占的格子
	switch (m_dir)
	{
	case UP:    y = Y+1; break;
	case DOWN:  y = Y-1; break;
	case LEFT:  x = X+1; break;
	case RIGHT: x = X-1; break;
	}
	color = m_color;
	if ((*map)[y][x] == WALL_A) //草地
		color |= wall_color[WALL_A] ;
	writeChar(y, x, blood_pic[m_blood], color);

	map->putRes(y, x, m_id); 

}

//坦克地图和控制台上的坦克
void Tank::ClsTank(Map *map)
{
	byte(*tankArr)[3] = tank_style[0][m_dir];
	int tmp;

	for (int row = -1; row < 2; row++)
	{
		for (int col = -1; col < 2; col++)
		{
			int y = Y + row; //行坐标
			int x = X + col; //列坐标

			tmp = tankArr[row + 1][col + 1];
			if (tmp)
			{
				//printf("%d", tmp);
				map->movRes(y, x);
				tmp = (*map)[y][x];
				writeChar(y, x, wall_pic[tmp], wall_color[tmp]);
			}
		}
	}
	int y = Y;
	int x = X;
	//清除血量占的格
	switch (m_dir)
	{
	case UP:    y = Y + 1; break;
	case DOWN:  y = Y - 1; break;
	case LEFT:  x = X + 1; break;
	case RIGHT: x = X - 1; break;
	}
	map->movRes(y, x);
	tmp = (*map)[y][x];
	writeChar(y, x, wall_pic[tmp], wall_color[tmp]);
}

//坦克的碰撞检测
bool Tank::checkTank(Map *map, bool isMov)
{

	int ym = 0, xm = 0;
	if (isMov)
	{
		switch (m_dir)
		{
		case UP:    ym--; break;
		case DOWN:  ym++; break;
		case LEFT:  xm--; break;
		case RIGHT: xm++; break;
		}
	}
	int tmp;
	byte(*tankArr)[3] = tank_style[0][m_dir];

	for (int row = -1; row<2; row++)
	{
		for (int col = -1; col<2; col++)
		{

			int y = row + Y + ym;
			int x = col + X + xm;

			tmp = tankArr[row + 1][col + 1];
			if (tmp)//如果是坦克身体的一部分
			{
				if ((*map)[y][x] != WALL__ &&
					(*map)[y][x] != WALL_A)
				{
					return false;
				}
				if (map->isPlaced(x, y))
				{
					return false;
				}
			}
		}
	}
	return true;
}

//初始化坦克（随机一个坐标，血量为满血，无敌状态)
void Tank::init(Map *map, int blood)
{
	do
	{
		X = rand() % (MAP_WIDTH - 4) + 2;
		Y = rand() % (MAP_HIGH - 4) + 2;
	} while (!checkTank(map, false));
	m_stat = inv;
	if (blood)
		m_blood = m_bloodMAX = blood;
	else
		m_blood = m_bloodMAX;

	DrawTank(map);
}

//移动坦克
void Tank:: moveTank(Map *map, int orientation) 
{

	//1.清除旧轨迹
	ClsTank(map);//清除移动前的tank
	//2.改变坦克位置
	//3.画出新轨迹
	int tmp = m_dir; 

	if (orientation != m_dir)
	{
		
		m_dir = orientation;

		/*
		//如果不能转向,则先移动
		if (!checkTank(temp))
		{
		temp.orientation = pTank->orientation;
		goto move;
		}
		*/
		if (checkTank(map, false))//如果可以转向直接转向，不可以则移动加转向
		{
			goto end;
		}
	}
	if (checkTank(map, true))//如果可以移动
	{
		switch (m_dir)
		{
		case UP:    Y--; break;
		case DOWN:  Y++; break;
		case LEFT:  X--; break;
		case RIGHT: X++; break;
		}
	}
	else
	{
		m_dir = tmp;
	}
	
end :
	DrawTank(map);
}

//开火
void Tank::fire(Missile::MissileFactory &mf, list<Missile*> &missList, Map *map)
{
 	clock_t current_time = clock();
	if (current_time - m_time < 260)
		return;
	m_time = current_time;
	Missile *tmp;
	if((tmp = mf.buyMissile(m_id, m_dir, *this, map, m_color)))
		missList.push_back(tmp);
}

//判断是不是自己
bool Tank:: isMy(unsigned int id)
{
	return m_id == id;
}

//自动驾驶
void Tank::Unmanned(Missile::MissileFactory &mf, list<Missile*> &missList, Map *map, Player *player)
{
	
	int oper ;
	if (m_stat == act)
	{
		g_Game->Astart().setElement(*this, *player);
		oper = g_Game->Astart().findPath(*this);
	} else if (m_stat == inv)
	{
		oper = rand() % 16;
	}
	
	if (oper == -1)
		oper = rand() % 16;
	
	switch (oper)
	{
	case UP:    moveTank(map, UP);   break;
	case DOWN:  moveTank(map, DOWN);   break;
	case LEFT:	moveTank(map, LEFT);   break;
	case RIGHT:	moveTank(map, RIGHT);   break;
	default:
		moveTank(map, m_dir);
	}
	if (X == player->X || Y == player->Y)
	{
		if (m_stat == inv)
		{
			m_stat = act;
			return;
		}
		fire(mf, missList, map);	
	}
}

//受伤减血死亡判断
bool Tank::wounded()
{
	if (m_stat == inv)
	{
		m_stat = act;
		return false;
	}
	m_blood--;
	if (m_blood) //是否死亡
		return false;
	m_stat = die;
	return true;
}

//返回该坦克的荣誉分数
int Tank :: Score()
{
	return m_score;
}

Tank::~Tank()
{
}
