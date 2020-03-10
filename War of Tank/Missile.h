#pragma once
#include <Windows.h>
#include "Map.h"


#define MAX 9999
#include <vector>
using std::vector;
#include <list>
using std::list;

class Missile
	:protected COORD
{
protected:
	unsigned int m_id;          //导弹编号

	int m_dir;		//朝向
	int m_max;		//血量
	int m_style;    //子弹样式
	bool m_isValid;   //是否有效
	bool m_isfly ;//是否斜着飞
	unsigned int m_color;

	unsigned int m_tankID;				//坦克编号（记录这颗导弹是谁发射的）

public:
	Missile(unsigned int id);
	Missile(unsigned int id, unsigned int tankID, int max, int m_style);

	virtual bool init(unsigned int tankId, int dir, int x, int y, unsigned int color);
	bool init(unsigned int tankId, int dir, int x, int y, unsigned int color, int max, bool isfly);
	bool init(unsigned int tankId, int dir, int x, int y, unsigned int color, int max, bool isfly, int);

	void hitWall(int row, int col);
	void hitTank(int x, int y);
	void hitMissile(int x, int y);
	virtual bool checkMissile();
	virtual void ClsMissile(Map *map);
	virtual int DrawMissile(Map *map);
	bool moveMissile(Map *map);

	~Missile();
protected:

public:
#define MMIN 60
	class MissileFactory
	{
	private:
		static unsigned int MISSID;
		vector<Missile *> m_missilePool;
		void clearPool();
	public:
		MissileFactory();
		void recMissile(Missile *miss);
		Missile *buyMissile(int tankID, int dir, COORD &pos, Map *map, unsigned int color);
		Missile *buyMissile(int tankID, int dir, COORD &pos, Map *map, unsigned int color, int max, int isfly);
		Missile *buyMissile(int tankID, int dir, COORD &pos, Map *map, unsigned int color, int max, int isfly, int);
		~MissileFactory();
	};


};





