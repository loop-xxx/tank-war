#include "AStart.h"
#include "Player.h"
#include <iostream>
using namespace std;

AStart::AStart(Map *map, int width, int height)
	:m_width(width), m_height(height), m_map(map), m_dest({ 0, 0 })
{
	if (m_width <= 0 || m_height <= 0 || m_map == nullptr)
	{
		throw "AStart init error";
	}
}

int AStart::_findNext() //访问已扩展点中权重最小的点并将其放入以扩展点中
{
	if (m_extendList.empty())
	{
		return -1;
	}
	int loss = m_extendList[0]->getLoss();
	int tmp, index = 0;
	for (int i = 1; i < m_extendList.size(); i++) //找到扩展列表中消耗最小的点
	{
		if (loss > (tmp = m_extendList[i]->getLoss()))
		{
			loss = tmp;
			index = i;
		}
	}
	m_visitedList.push_back(m_extendList[index]);//将其放入以访问列表
	m_extendList.erase(m_extendList.begin() + index);

	COORD coord;
	m_visitedList.back()->getCOORD(coord);
	for (int row = -1; row<2; row++)
	{
		for (int col = -1; col<2; col++)
		{

			int y = row + m_dest.Y;
			int x = col + m_dest.X;

			if (coord.X == x && coord.Y == y) //如果找到终点直接返回
				return 1;
		}
	}

	return 0;
}

int AStart::_checkPos(COORD &coord, int weight) //判断该坐标点是否可以扩展
{
	for (int i = 0; i < m_visitedList.size(); i++) //判断该点是否已经访问过
	{
		if (m_visitedList[i]->comperTo(coord))
			return -1;
	}
	if (coord.X < 1 || coord.Y < 1 || coord.X > m_width - 1 || coord.Y > m_height - 1) //判断要扩展的坐标是否越界
	{
		return -1;
	}

	int tmp;
	for (int row = -1; row<2; row++)
	{
		for (int col = -1; col<2; col++)
		{

			int y = row + coord.Y;
			int x = col + coord.X;

			if ((tmp = (*m_map)[y][x]) != WALL__ && tmp != WALL_A) //判断要扩展的坐标是否有障碍物
			{
				return -1;
			}
		}
	}

	
	

	for (int i = 0; i < m_extendList.size(); i++)
	{
		if (m_extendList[i]->comperTo(coord))
		{
			m_extendList[i]->updateLoss(weight);
			return 1; //如果已扩展的点中有该点则更新该点的信息
		}
	}
	

	return 0;
}

void AStart::_extNext() //扩展点
{
	COORD tmp, coord;
	Position *pos = m_visitedList.back();
	pos->getCOORD(tmp);
	int weight = pos->getWeigth();
	//UP
	coord.X = tmp.X;
	coord.Y = tmp.Y - 1;
	if(!_checkPos(coord, weight))
		m_extendList.push_back(new Position(coord, weight, Position::calcLoss(coord, m_dest, weight), pos));
	//DOWN
	coord.X = tmp.X;
	coord.Y = tmp.Y + 1;
	if (!_checkPos(coord, weight))//为零表示可以扩展
		m_extendList.push_back(new Position(coord, weight, Position::calcLoss(coord, m_dest, weight), pos));
	//LEFT
	coord.X = tmp.X - 1;
	coord.Y = tmp.Y;
	if (!_checkPos(coord, weight))
		m_extendList.push_back(new Position(coord, weight, Position::calcLoss(coord, m_dest, weight), pos));
	//right
	coord.X = tmp.X + 1;
	coord.Y = tmp.Y;
	if (!_checkPos(coord, weight))
		m_extendList.push_back(new Position(coord, weight, Position::calcLoss(coord, m_dest, weight), pos));
}

int AStart::_findPath()
{
	
	int code;
	while (!(code = _findNext()))//获取最小消耗的已访问点，通过该点扩展
	{
		_extNext();
	}
	return code == -1 ? 1 : 0;
}

/*****************外部接口*****************/
void AStart::setElement(const COORD &start, const COORD &dest) //设置起点和终点
{
	m_extendList.push_back(new Position(start, 0, Position::calcLoss(start, dest, 0), nullptr));
	m_dest = dest;
}


int AStart::findPath(COORD &src)
{
	list<COORD> path;
	int code;
	COORD tmp;
	if (!(code = _findPath()))
	{
		Position *pos = m_visitedList.back();
	do{
		pos->getCOORD(tmp);
		path.push_front(tmp);
	} while (pos = pos->m_prev) ;
	}
	for (int i = 0; i < m_extendList.size(); i++)
		delete m_extendList[i];
	
	for (int i = 0; i < m_visitedList.size(); i++)
		delete m_visitedList[i];

	m_visitedList.clear();
	m_extendList.clear();
	if (path.size() < 2)
		return -1;
	path.pop_front();
	COORD end = path.front();
	if (src.Y - end.Y == 1)
	{
		return UP;
	}
	else if (src.Y - end.Y == -1)
	{
		return DOWN;
	}
	else if (src.X - end.X == 1)
	{
		return LEFT;
	}
	else if (src.X - end.X == -1)
	{
		return RIGHT;
	}

	return -1;
}

AStart::~AStart()
{
}
