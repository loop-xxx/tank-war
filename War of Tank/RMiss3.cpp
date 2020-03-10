#include "RMiss3.h"
#include "Engine.h"

RMiss3::RMiss3()
	:RBase(0x3)
{
}

bool RMiss3::run(Map *map, Player *play)
{
	static clock_t start_time = 0;
	clock_t current_time = clock();
	if (current_time - start_time < 34)
		return false;
	start_time = current_time;

	return moveMissile(map);
}

bool RMiss3::checkMissile()
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
	int col ;
	int row ;
	int i = 0; 
	while (i >= -1)
	{
		switch (m_dir)
		{
		case UP:
			col = x + i;
			if (i != 0)
				row = y + 1;
			else
				row = y;
			break;
		case DOWN:
			col = x + i;
			if (i != 0)
				row = y - 1;
			else
				row = y;
			break;
		case LEFT:
			row = y + i;
			if (i != 0)
				col = x + 1;
			else
				col = x;
			break;
		case RIGHT:
			row = y + i;
			if (i != 0)
				col = x - 1;
			else
				col = x;
			break;
		}
		int tmp;
		if ((tmp = (*g_Game->m_map)[row][col]) == WALL_D || tmp == WALL_A)
		{
			return false;
		}
		else if (tmp == WALL_C)
		{
				hitWall(row, col);
		}
		else if (tmp != WALL__ && tmp != WALL_E)
		{
			hitWall(row, col);
		}
		else if (g_Game->m_map->isPlaced(col, row))
		{
			if (g_Game->m_map->getRecID(col, row) <0x60)
			{
				hitTank(col, row);
			}
			else
			{
				hitMissile(col, row);
			}
		}
		if (i)
			i -= 2;
		else
			i = 1;
	}

	return true;
}

int RMiss3::DrawMissile(Map *map)
{
	
	map->putRes(Y, X, m_id);
	int col ;
	int row ;
	for (int i = -1; i < 2; i++)
	{
		switch (m_dir)
		{
		case UP: 
			col = X + i; 
			if (i != 0)
				row = Y + 1;
			else
				row = Y;
			break;
		case DOWN:  
			col = X + i;
			if (i != 0)
				row = Y - 1;
			else
				row = Y;
			break;
		case LEFT:
			row = Y + i;
			if (i != 0)
				col = X + 1;
			else
				col = X;
			break;
		case RIGHT:
			row = Y + i;
			if (i != 0)
				col = X - 1;
			else
				col = X;
			break;
		}
			
		char * pic = missile_pic[m_style+i];

		writeChar(row, col, pic, m_color);

		//草地的显示等级比子弹高（地图元素封装成类后，可以直接查看其显示等级）
		if ((*map)[row][col] == WALL_A) //草地
		{
			writeChar(row, col, pic, wall_color[WALL_A] | m_color);
		}
		else if ((*map)[row][col] == WALL_E) //河流
		{
			writeChar(row, col, pic, wall_color[WALL_D] | m_color);
		}
	}
	return true;
}

void RMiss3::ClsMissile(Map *map)
{
	map->movRes(Y, X);
	
	int col ;
	int row ;
	for (int i = -1; i < 2; i++)
	{
		switch (m_dir)
		{
		case UP:
			col = X + i;
			if (i != 0)
				row = Y + 1;
			else
				row = Y;
			break;
		case DOWN:
			col = X + i;
			if (i != 0)
				row = Y - 1;
			else
				row = Y;
			break;
		case LEFT:
			row = Y + i;
			if (i != 0)
				col = X + 1;
			else
				col = X;
			break;
		case RIGHT:
			row = Y + i;
			if (i != 0)
				col = X - 1;
			else
				col = X;
			break;
		}
		int tmp = (*map)[row][col];
		writeChar(row, col, wall_pic[tmp], wall_color[tmp]);
	}
}

RMiss3::~RMiss3()
{
}
