#include "global.h"
#include "engine.h"
#include <string.h> 
#include <fstream>  
#include <exception>
#include <iostream>
using namespace std;


//列表位置
#define TOP 4
#define EDGE MAP_WIDTH+1
//列表
#define CHOOSE_COUNT 6
#define OPT_COUNT 4 
static char* chooseArr[CHOOSE_COUNT] = { "\"  \" 空道（不可破话，  可通过）", "\"≡\" 丛林（不可破坏，  可通过，可以隐身）" ,"\"卍\" 土墙（  可破坏, 不可通过，土墙的破坏形态）", "\"▓\" 土墙（  可破坏，不可通过）",  "\"〓\" 铁墙 (不可破坏, 不可通过)", "\"≈\" 河流（不可破坏,   子弹可通过，坦克不可通过）" };
char *optArr[OPT_COUNT] = { "保存地图", "使用地图", "读取地图", "返回" };
static void editMAP() //编辑地图
{
	fstream fp ;
	Map *map = new Map();
	COORD pos;
	char buf[20]{};
	int option = 0;
begin:map->drawMap();

	showChoose(TOP, EDGE, CHOOSE_COUNT, chooseArr, F_H_WHITE, option);
	showMessage("正在编辑地图。。。", F_H_WHITE, BOTTOM);
	writeChar(TOP + CHOOSE_COUNT + 3, EDGE+4, "点击列表选择墙体", F_H_WHITE);
	writeChar(TOP + CHOOSE_COUNT + 4, EDGE + 4, "点击空区域显示编辑菜单", F_H_WHITE);
	while (true)
	{
		if (getMousePos(pos) == 1)
		{
			if (pos.X > MAP_WIDTH && pos.Y >= TOP && pos.Y < TOP + CHOOSE_COUNT)
			{
				option = pos.Y - TOP;
				showChoose(TOP, EDGE, CHOOSE_COUNT, chooseArr, F_H_WHITE, option);
			}
			else if (pos.X < MAP_WIDTH - 1 && pos.X > 0 && pos.Y > 0 && pos.Y < MAP_HIGH - 1)
			{
				writeChar(pos.Y, pos.X, wall_pic[option], wall_color[option]);
				(*map)[pos.Y][pos.X] = option;
			}
			else if (pos.X > MAP_WIDTH + 4 && pos.Y > 16)
			{
				switch (showMenu(OPT_COUNT, optArr))
				{
				case 0:
					printf("------------------------------------\n");
					printf("请输入地图名:");
					scanf_s("%s", buf, sizeof buf - 4);
					strcat_s(buf,sizeof buf, ".wtm");
					fp.open(buf, ios::out | ios::binary);
					if (!fp.is_open())
					{
						cout << "fail to open file" << endl;
						cout << "Please check the file name" << endl;
						system("pause");
						goto begin;
					}
					fp.write((char*)map, sizeof(Map));
					fp.close();
					cout << "save success" << endl;
					system("pause");
					goto begin;
					break;
				case 1:
					g_Game->setMap(map);
					showMessage("地图获取成功！", F_H_WHITE, BOTTOM);
					system("pause");
					goto end;
				case 2:
					if (showFileMenu("./*.wtm", buf, sizeof buf))
					{
						showMessage("读取失败!(请检查是否有地图)", F_H_WHITE, BOTTOM);
						system("pause");
						goto begin;
					}

					fp.open(buf, ios::in | ios::binary);
					fp.read((char*)map, sizeof(Map));
					fp.close();
					goto begin;
					
					break;
				case 3:
					goto begin;
					break;
				}
			}
		}
		
		sprintf_s(buf, "< X = %d， Y = %d >", pos.X, pos.Y);
		writeChar(TOP - 3, EDGE, buf, F_H_WHITE);
	}
	end:;
}

//读取一个数字
static int getNumber(void)
{
	int tmp;
	while (scanf_s("%d", &tmp) != 1)
	{
		while (getchar() != '\n');
		printf("error\n");
	}
	while (getchar() != '\n');
	return tmp;
}
#define NPCM 6 //场上敌军最多是
static void editNPC()
{
	int max, totle;
	while (true)
	{
		printf("------------------------------------\n");
		printf("请输入敌军总数量:");
		max = getNumber();
		printf("请输入场上敌军总数量:");
		while ((totle = getNumber()) > NPCM)
			printf("error: not > %d\n", NPCM);
		printf("ENTER TO CONTINUE OR L TO LOOP\n");
		switch (getKeyCode())
		{
		case VK_RETURN:
			goto end;
		case 'L':
			break;
		}
	}
end:g_Game->setOption(max, totle);
}


#define ATTR_COUNT 2
char *attrArr[ATTR_COUNT]{"重甲(血:高,速:慢)", "轻甲(血:低,速:快)"};
#define TYPE_COUNT 4
char *typeArr[TYPE_COUNT]{"激光炮", "精准弹幕", "强力弹", "扩散弹"};
static void editPlayer()
{
	int blood;
	int speed; 
	unsigned int color;
	int type;
	if (showMenu(ATTR_COUNT, attrArr))
	{
		blood = 8;
		speed = 44;
		color = F_H_YELLOW;
	}
	else
	{
		blood = 10;
		speed = 60;
		color = F_H_PURPLE;
	}
	type = showMenu(TYPE_COUNT, typeArr);

	Player *p = new Player(color, blood,speed,type);
	g_Game->setPlayer(p);
}

//显示编辑菜单
#define MENU_COUNT 4
static char *menuArr[MENU_COUNT]{"编辑地图", "调整敌方", "调整武装", "返回"};
static void(*funArr[MENU_COUNT])(void) { editMAP, editNPC, editPlayer};
Engine* Edit()
{
	if (!g_Game)
		g_Game = new Engine();
	int index;
	while ((index = showMenu(MENU_COUNT, menuArr)) != MENU_COUNT -1)
	{
		funArr[index]();
	}
	
	return g_Game;
}
