#pragma once
#include "ConsoleTool.h"
#include "Missile.h"
#include "Map.h"
#include <vector>
#include <list>

class Player;

#define MIN 4 
using std::vector;
using std::list;

#define DEFBLOOD 3
#define DEFSCORE 4

enum TankStat
{
	inv , act, die
};
class Tank
	:protected COORD
{
protected:
	WORD m_color;//颜色默认的坦克颜色是蓝色的
	int m_bloodMAX;//血量上限默认为DEFBLOOD
	int m_blood;//血量
	byte m_dir; //方向
	bool checkTank(Map *map, bool isMov);
	clock_t m_time = 0;
	unsigned int m_id; //id
	TankStat m_stat; //状态
	int m_score; //分数

public:
	Tank(unsigned int id);
	Tank(unsigned int id, WORD color, int boolMAX, int score);

	void ClsTank(Map *map);
	void DrawTank(Map *map);
	virtual void init(Map *map, int blood);//初始化坦克/***********随机出合适的坐标并将坦克画在地图上*************/
	void moveTank(Map *map, int orientation);
	void fire(Missile::MissileFactory &mf, list<Missile*> &missList, Map *map);
	bool isMy(unsigned int id);
	virtual bool wounded();
	void Unmanned(Missile::MissileFactory &mf, list<Missile*> &missList, Map *map, Player *player);
	int Score();
	~Tank();
public:

	class TankFactory
	{
	private:
		vector<Tank *> tankPool;
		static unsigned int TANKID;
		void clearPool();
		int m_blood = DEFBLOOD;
	public:
		TankFactory();
		Tank *buyTank(Map *map);
		void recTank(Tank *tank);
		void updateTank(int levl);
		~TankFactory();
	};
};

