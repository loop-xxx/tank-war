#include "Position.h"

/*******************初始化方法***********************/

Position::Position(const COORD& pos, int weight, int loss, Position *prev)
	: COORD(pos), m_weight(weight), m_loss(loss), m_prev(prev)
{
}


 /****************************获取该点的坐标********************************/
 void Position:: getCOORD(COORD &coord)
 {
	 coord.X = X;
	 coord.Y = Y;
 }

 /********************外部接口**********************/
 bool Position:: comperTo(const COORD &coord) //判断是不是同一个坐标
 {
	 if (X == coord.X && Y == coord.Y)
	 {
		 return true;
	 }
	 return false;
 }

 int Position::getLoss() //获取该点直接到达终点的最小消耗
 {
	 return m_loss;
 }

 void Position ::updateLoss(int weight) //更新该点直接到达终点的最小消耗
 {
	 if (m_weight > weight)
	 {
		 m_loss -= (m_weight - weight);
		 m_weight = weight;
	 }
 }

 int Position::getWeigth()//获取当前点的权重
 {
	 return m_weight;
 }

Position::~Position()
{
}

int Position::calcLoss(const COORD& src, const COORD& dest, int weight) //计算最小消耗
{
	return abs(dest.X - src.X) + abs(dest.Y - dest.Y) + weight;
}
