#pragma once

#include <vector>
#include <list>
#include "Position.h"
#include "Map.h"
using std::vector;
using std::list;

class AStart
{
private:
	/******地图信息*****/
	int m_width;
	int m_height;
	Map *m_map ;
	/*********点信息**********/
	COORD m_dest;
	/***********访问信息**************/
	vector<Position *> m_extendList; //存放扩展的点
	vector<Position *> m_visitedList; //存放访问过的点

public:
	AStart(Map *map , int width, int height);
	void setElement(const COORD &start, const COORD &dest);
	int findPath(COORD &src);
	~AStart();
private:
	int _checkPos(COORD &coord, int weight);//判断该坐标点是否可以扩展
	void _extNext();//扩展新的点
	int _findNext();//找到以扩展点中消耗最小的作为下一个新的扩展的基点
	int _findPath();//获取路径
};

