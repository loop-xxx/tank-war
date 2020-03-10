#include "global.h"
#include "Engine.h"
#include "ConsoleTool.h"

Engine *g_Game = nullptr;

extern Engine *Edit();
static void Start()
{
	if (g_Game == nullptr)
		g_Game = new Engine();
	g_Game->Start();
}
#define MENU_COUNT 4
static char* menuArr[MENU_COUNT] {"开始游戏", "排行榜", "游戏编辑器", "退出游戏"};

#define TOP_COUNT 6
static char *topArr[TOP_COUNT] {"loop\t99296", "loop\t96432", "loop\t12121", "loop\t11013", "loop\t10023", "loop\t9139"};
static void Level()
{
	showMenu(TOP_COUNT, topArr);
}

int main(int argc, char *argv[])
{	
	initConsole();
	while (true)
	{
		switch (showMenu(MENU_COUNT, menuArr))
		{
		case 0:
			Start();
			break;
		case 1:
			Level();
			break;
		case 2:
			Edit();
			break;
		case 3:
			exit(0);
			break;
		default:
			break;
		};
	}
	
	

	return 0;
}