#include "Player.h"
#include "RMiss4.h"
#include "RMiss3.h"
#include "RMiss2.h"
#include "RMiss1.h"
#include "Engine.h"
 
#define GET_BLOOD 2
#define GET_RCOUNT 6
#define SPEED_UP 2
Player::Player()
	:Tank(PID, F_H_PURPLE, 10, 0), m_speed(60), m_Rcount(RCOUNT), m_R(nullptr), m_type(0)
{

}

Player::Player(unsigned int color, int bloodMAX, clock_t speed, int type)
	: Tank(PID, color, bloodMAX, 0), m_speed(speed), m_type(type), m_Rcount(RCOUNT), m_R(nullptr)
{
}

//根据血量的最大值恢复血量和速度的默认值
void Player::init(Map *map)
{
	Tank::init(map,0);
	m_Rcount = RCOUNT;
	m_score = 0;
	if (m_bloodMAX == 10)
	{
		m_speed = 60;
	} else if (m_bloodMAX == 8)
	{
		m_speed = 44;
	}

}

//监听按键玩家操作坦克
void Player::Drive(Map *map, Missile::MissileFactory &mf, list<Missile*> &missList)
{
	static clock_t start_time = 0;
	clock_t current_time = clock();
	if (current_time - start_time < m_speed)
		return;
	start_time = current_time;
	if (KEY_DOWN(VK_UP))    { moveTank(map, UP); }
	if (KEY_DOWN(VK_DOWN))  { moveTank(map, DOWN); }
	if (KEY_DOWN(VK_LEFT))  { moveTank(map, LEFT); }
	if (KEY_DOWN(VK_RIGHT)) { moveTank(map, RIGHT); }
	if (KEY_DOWN('X')) { fire(mf, missList, map); }

	if (KEY_DOWN('C')){ R(map); };
}

//重写了坦克的受伤函数，当玩家受伤时会反弹子弹
bool Player::wounded()
{
	if (m_stat == inv)
	{
		showMessage("护罩失效！", F_H_RED, 0);
		m_stat = act;
		return false;
	}
	Muisc("wounded.wav");
	Missile *tmp;
	for (int i = LEFT; i <= DOWN; i++)
	{
		if (tmp = g_Game->m_mf.buyMissile(PID, i, *this, g_Game->m_map,m_color, 4, true, 0))
			g_Game->m_MISSlist.push_back(tmp);
		if (tmp = g_Game->m_mf.buyMissile(PID, i, *this, g_Game->m_map,m_color, 4, false, 0))
			g_Game->m_MISSlist.push_back(tmp);
	}
	
	showMessage("护甲损坏！", F_H_RED, 0);
	m_blood--;
	m_speed -= SPEED_UP;
	if (m_blood) //是否死亡
		return false;
	m_stat = die;
	return true;
}

//获取分数
void Player::get(Tank *tank)
{
	m_score += tank->Score();
}

//返回玩家状态是否为死亡
bool Player::isOver()
{
	return m_stat == die;
}

//打印玩家信息，血量，荣誉分数等
void Player::showInfo()
{
	static clock_t start_time = 0;
	clock_t current_time = clock();
	if (current_time - start_time < 400)
		return;
	start_time = current_time;
	char buf[20];
	sprintf_s(buf,"血量:%d", m_blood);
	showMessage(buf, F_H_YELLOW, 1);
	sprintf_s(buf, "元力%d", m_Rcount);
	showMessage(buf, F_H_YELLOW, 2);
	sprintf_s(buf, "荣誉%d", m_score);
	showMessage(buf, F_H_YELLOW, 3);
}

//玩家获得补给，血量恢复，元力上升
void Player ::upgrade(Map *map)
{
	Muisc("upgrade.wav");
	if ((m_blood + GET_BLOOD) <= m_bloodMAX)
	{
		m_blood += GET_BLOOD;
		m_speed += SPEED_UP*GET_BLOOD;
	}
	else
	{
		m_speed += (m_bloodMAX - m_blood) * SPEED_UP;
		m_blood = m_bloodMAX;

	}
	m_Rcount += GET_RCOUNT;
	m_stat = inv;
	DrawTank(map);
}

//释放大招
void Player::R(Map *map)
{
	if (m_R) //如果大招正在释放
		return;

	if (!m_Rcount)//如果没有元力
		return;
	static clock_t start_time = 0;
	clock_t current_time = clock();
	if (current_time - start_time < 600)
		return;
	start_time = current_time;//如果时间间隔不够

	m_Rcount--;
	int x = X, y = Y;
	switch (m_dir)
	{
	case UP:    y--; break;
	case DOWN:  y++; break;
	case LEFT:  x--; break;
	case RIGHT: x++; break;
	}
	Muisc("r.wav");
	switch (m_type) //根据玩家设定的技能选择生成大招
	{
	case 0:
		m_R = new RMiss1();
		break;
	case 1:
		m_R = new RMiss2();
		break;
	case 2:
		m_R = new RMiss3();
		break;
	case 3:
		m_R = new RMiss4();
		break;
	}
	
	if (!m_R->init(m_id, m_dir, x, y, m_color))//初始化大招，如果大战直接打到墙直接消失
	{
		delete m_R;
		m_R = nullptr;
	}

}

//打印玩家信息，监听按键， 大招运行
void Player::Run(Missile::MissileFactory &mf, list<Missile*> &list, Map *map)
{
	showInfo();
	Drive(map, mf, list);
	if (!m_R)//如果没有大招
		return;
	if (m_R->run(map, this))//释放了大招则运行大招，run返回false表示大招运行完毕释放大招内存
	{
		delete m_R;
		m_R = nullptr;
	}

}

Player::~Player()
{
}
