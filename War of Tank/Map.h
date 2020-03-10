#pragma once
#include "ConsoleTool.h"
#include <exception>

using std::exception;

#define  MAP_HIGH 40
#define  MAP_WIDTH 40

typedef byte(*MAP)[MAP_WIDTH];


class Map
{
private:
	static const byte G_MAP[MAP_HIGH][MAP_WIDTH];

	byte m_map00[MAP_HIGH][MAP_WIDTH]; //坦克地图
	byte m_map01[MAP_HIGH][MAP_WIDTH]; //墙体地图

public:
	Map();
	Map(const MAP map01);
	Map(const Map &map);
	Map& operator=(const Map &map);
	byte *operator[](int index);
	void drawMap(void);
	void putRes(int row, int col, byte id);
	void movRes(int row, int col);
	bool isPlaced(int x, int y);
	unsigned int getRecID(int x, int y);
	void testdrawMap(void);
	~Map();
};

