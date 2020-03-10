#include "Engine.h"

Engine::Engine()
	:m_currentNPC(DEF_CNPC), m_maxNPC(DEF_NPCMAX), m_player(nullptr), m_map(nullptr), m_speed(DEF_SPEED), m_tmp(0), m_isPause(false)
{
}


//运行游戏
void Engine::run()
{
	while (true)
	{
		if (KEY_DOWN('P')){ m_isPause = true; } 
		//监听操作玩家，大招运行
		m_player->Run(m_mf, m_MISSlist, m_map);
		//检验NPC数量
		checkNPC();
		//移动子弹
		movAllMissile();
		//移动坦克
		movAllTank();
		//检查拾取
		checkCime();
		//检查地图等级
		checkLevel();
		//检查暂停
		checkPause();
		//检查结束
		if (checkOver())
			break;
	}
}

//检查是否结束
bool Engine::checkOver()
{
	bool flag = false;
	static clock_t start_time = 0;
	clock_t current_time = clock();
	if (current_time - start_time < 1000)
		return false;

	if (m_player->isOver()) //玩家死亡
	{
		auto itor = m_NPClist.begin();//清理NPC
		while (itor != m_NPClist.end())
		{
			(*itor)->ClsTank(m_map);
			m_tf.recTank(*itor);
			itor++;
		}
		m_NPClist.clear();
		flag = true;
		
	} 
	else if ((m_maxNPC == 0 && m_NPClist.size() == 0))//NPC为0
	{
		flag = true;
	} 

	//如果结束就清理场上所有的子弹
	if (flag)
	{
		auto itor = m_MISSlist.begin();
		while (itor != m_MISSlist.end())
		{
			(*itor)->ClsMissile(m_map);
			m_mf.recMissile((*itor));
			itor++;
		}
		m_MISSlist.clear();
		showDialog("     游  戏   结  束     ");
	}

	return flag;
} 

//检查暂停
void Engine::checkPause()
{
	static clock_t start_time = 0;
	clock_t current_time = clock();
	if (current_time - start_time < 400)
		return ;
	start_time = current_time;
	if (m_isPause)
	{
		showDialog("     游  戏   暂  停     ");
		m_isPause = false;
	}
}

//移动所有子弹
void Engine:: movAllMissile()
{
	static clock_t start_time = 0;
	clock_t current_time = clock();
	if (current_time - start_time < 40)
		return;
	start_time = current_time;
	auto itor = m_MISSlist.begin();
	while (itor != m_MISSlist.end())
	{
	
 		if ((*itor)->moveMissile(m_map))
		{
			m_mf.recMissile((*itor));
			itor = m_MISSlist.erase(itor);
		}
		else
		{
			itor++;
		}
	}
} 

//移动所有坦克
void Engine::movAllTank()
{
	static clock_t NPC_clockStart;
	clock_t NPC_clockFinish = clock();
	if (NPC_clockFinish - NPC_clockStart < m_speed) { return; }
	NPC_clockStart = NPC_clockFinish;
	auto itor = m_NPClist.begin();
	while (itor != m_NPClist.end())
	{
		(*itor)->Unmanned(m_mf, m_MISSlist, m_map, m_player);
		itor++;
	}
}

//检查刷新NPC
void Engine::checkNPC()
{
	static clock_t start_time = 0;
	int current_time = clock();
	if (current_time - start_time < 2000)
		return;
	start_time = current_time;
	Tank *tmp;
	if (m_NPClist.size() < m_currentNPC)
	{
		if (m_maxNPC)
		{
			m_maxNPC--;
			tmp = m_tf.buyTank(m_map);
			m_NPClist.push_back(tmp);
		}
	}
	char buf[26];
	sprintf_s(buf, "战场上的敌人还有%d\n", m_currentNPC);
	showMessage(buf, F_H_RED, 14);
	sprintf_s(buf, "伺机待发的敌人还有%d\n", m_maxNPC);
	showMessage(buf, F_H_RED, 12);
}

//根据玩家的荣誉分数增加战场等级
void Engine::checkLevel()
{
	
	static clock_t start_time = 0;
	int current_time = clock();
	if (current_time - start_time < 2000)
		return;
	start_time = current_time;
	if ((m_player->Score() - m_tmp) > 100)
	{
		m_tmp = m_player->Score();
		m_level++;
		m_speed -= 4;
		m_tf.updateTank(m_level);
		char buf[20];
		sprintf_s(buf,"第%d层", m_level);
		showMessage(buf, F_H_GREEN, 20);
		showMessage("战场难度提升！", F_H_GREEN, 0);
	}
	
}

//检查补给，是否生成补给，是否给玩家补给
void Engine::checkCime()
{
	static clock_t start_time = 0;
	int current_time = clock();
	if (current_time - start_time < 40)
		return;
	start_time = current_time;
	if (!m_cime)
	{
		if (rand() % 200 != 0)
			return;
		int x, y;
		int tmp;
		while (true)
		{
			x = rand() % (MAP_WIDTH - 4) + 2;
			y = rand() % (MAP_HIGH - 4) + 2;
			if ((tmp = (*m_map)[y][x]) != WALL_D && tmp != WALL_E)
				break;
		}
		m_cime = (COORD *)malloc(sizeof(COORD));
		m_cime->X = x;
		m_cime->Y = y;
		writeChar(y, x, wall_pic[WALL_F], F_H_YELLOW | wall_color[(*m_map)[y][x]]);
		return;
	}

	if (m_map->getRecID(m_cime->X, m_cime->Y) == PID)
	{
		m_player->upgrade(m_map);
		showMessage("补给获取。。。", F_H_YELLOW, 0);
		free(m_cime);
		m_cime = nullptr;
		return;
	}
	writeChar(m_cime->Y, m_cime->X, wall_pic[WALL_F], F_H_YELLOW | wall_color[(*m_map)[m_cime->Y][m_cime->X]]);
}

//A星算法
AStart& Engine::Astart()
{
	return *m_Astart;
}

//初始化函数,恢复游戏的默认值
void Engine::Start()
{
	Muisc("begin.wav");
	showMessage("↑  ↓  ←  → 移动", F_H_WHITE, 6);
	showMessage("X 发射子弹，C 释放元力技能", F_H_WHITE, 7);
	showMessage("P 暂停", F_H_WHITE, 8);
	if (m_map == nullptr)
		m_map = new Map();
	if (m_player == nullptr)
		m_player = new Player();
	if (m_Astart)
		delete m_Astart;
	//初始化地图及AI
	m_Astart = new AStart(m_map, MAP_WIDTH, MAP_HIGH);
	m_map->drawMap();
	m_player->init(m_map);
	m_isPause = false;
	//恢复游戏等级
	m_level = 0; 
	m_tmp = 0;
	m_tf.updateTank(m_level);
	m_speed = DEF_SPEED;

	run();//运行游戏
} 

//设置敌人数量，编辑敌方中调用
void Engine::setOption(int maxNPC, int cNPC)
{
	m_maxNPC = maxNPC;
	m_currentNPC = cNPC;
}

//设置地图，地图编辑器中调用
void Engine::setMap(Map *map)
{
	if (m_map)
		delete m_map;
	m_map = map;
} 

//设置玩家，编辑武装中调用
void Engine::setPlayer(Player *palyer)
{
	if (m_player)
		delete m_player;
	m_player = palyer;
}

Engine::~Engine()
{
	if (m_map)
		delete m_map;
	if (m_player)
		delete m_player;
	delete m_Astart;
}
