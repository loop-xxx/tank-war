#pragma once
#include <Windows.h>

class Position
	:private COORD
{
private:
	int m_loss; //该点直接到达终点的最小消耗
	int m_weight; //到该点的权重

public:
	Position::Position(const COORD& pos, int m_weight, int loss, Position *prev);
	/****************外部接口*************/
	void getCOORD(COORD &coord);
	int getLoss();
	void updateLoss(int weight);
	int getWeigth();
	bool comperTo(const COORD &coord);

	static int calcLoss(const COORD& src, const COORD& dest, int weight); //计算消耗

	~Position();
public:
	Position *m_prev;
};

