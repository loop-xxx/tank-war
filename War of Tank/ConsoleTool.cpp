#include "ConsoleTool.h"
#include <windows.h>
#pragma comment(lib, "winmm.lib")
#include<io.h>
#include <vector>
using std::vector;

#define FILE_MAX 12 //文件最多数量

//窗口大小
#define WINDOWS_WIDTH 66
#define WINDOWS_HIGH  42
#define TITLE L"War of Tank"

HANDLE g_hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);    //获取标准输出句柄
HANDLE g_hStdIn = GetStdHandle(STD_INPUT_HANDLE);    //获取标准输出句柄

static bool setWindowSize(int width, int high)
{
	width = width * 2;  //把宽字符坐标，变为窄字符坐标

	//1. 获取最大控制台窗口大小
	COORD pos = GetLargestConsoleWindowSize(g_hStdOut);
	if (pos.Y <= high || pos.X <= width)
	{
		printf("你设置的窗口太大了，装不下！ \n");
		printf("请设置较小字体后，重新开始~ \n");
		return false;
	}
	COORD bufferSize = { pos.X, pos.Y };  //最大缓冲取大小
	pos.X -= 1;	pos.Y -= 1;              //最大窗口大小

	//2. 设置缓冲区大小(先设置为最大，以便设置窗口)
	if (!SetConsoleScreenBufferSize(g_hStdOut, bufferSize)) {
		printf("设置缓冲区大小失败 \n");
		return false;
	}

	//3. 确定窗口的最终大小
	width = width > pos.X ? pos.X : width;
	high = high > pos.Y ? pos.Y : high;

	//4. 设置窗口大小
	SMALL_RECT windowSize = { 0, 0, (short)width, (short)high };
	if (!SetConsoleWindowInfo(g_hStdOut, true, &windowSize)) {
		printf("设置窗口大小失败 \n");
		return false;
	}
	//5. 设置缓冲区大小
	bufferSize = { (short)width + 1, (short)high + 1 };
	if (!SetConsoleScreenBufferSize(g_hStdOut, bufferSize)) {
		printf("设置缓冲区大小失败 \n");
		return false;
	}

	return true;
}//设置窗口大小

void initConsole()
{
	srand((unsigned int)time(NULL));
	SetConsoleTitle(TITLE);
	if (!setWindowSize(WINDOWS_WIDTH, WINDOWS_HIGH))
	{
		system("pause");
		exit(-1);
	}
}

void clearConsle()
{
	SetConsoleTextAttribute(g_hStdOut, F_WHITE); //设置为黑白色
	system("CLS");
} //清空控制台

void writeChar(int row, int col, const char* pszChar, WORD wArr)
{
	// 设置光标属性
	CONSOLE_CURSOR_INFO cci;                    //控制台光标信息结构类型
	cci.dwSize = 1;                             //光标大小
	cci.bVisible = FALSE;                       //是否显示光标
	COORD loc;                                  //坐标结构类型
	loc.X = col * 2;                            //宽度必须*2
	loc.Y = row;                                //高度
	SetConsoleCursorInfo(g_hStdOut, &cci);      //设置指定控制台屏幕缓冲区光标大小和可见性
	SetConsoleCursorPosition(g_hStdOut, loc);   //设置指定控制台屏幕缓冲区中光标的位置
	SetConsoleTextAttribute(g_hStdOut, wArr);   //设置颜色
	//函数对函数调用后输出的文本产生影响
	printf(pszChar);                            //打印特殊字符占两个字节
}

//按键监听
byte getKeyCode()
{
	INPUT_RECORD inRec;        //定义输入事件结构体  
	DWORD res;      //定义返回记录  
	while (true)
	{
		ReadConsoleInput(g_hStdIn, &inRec, 1, &res);
		if (inRec.EventType == KEY_EVENT && inRec.Event.KeyEvent.bKeyDown)
			return inRec.Event.KeyEvent.wVirtualKeyCode;
		
	}
}//获取控制台按键事件阻塞

//显示菜单
byte showMenu(byte COUNT, char**menuArr)
{
	int row = (WINDOWS_HIGH - 5) / 2;
	int col = (WINDOWS_WIDTH - strlen("--------------") / 2) / 2;
	byte option = 0;

	clearConsle();
	writeChar(row - 1, col - 2, "---------------------", F_RED | B_GREEN);
	for (int i = 0; i < COUNT; i++)
	{
		writeChar(row + i, col, menuArr[i], F_WHITE);
	}
	writeChar(row + option, col - 2, "→", F_RED | B_H_WHITE);
	writeChar(row + COUNT, col - 2, "---------------------", F_RED | B_GREEN);

	while (true)
	{
		switch (getKeyCode())
		{
		case VK_UP:
			if (option >  0)
			{
				writeChar(row + option , col - 2, "　", F_RED);
				option--;
				writeChar(row + option, col - 2, "→", F_RED | B_H_WHITE);
			}
			break;
		case VK_DOWN:
			if (option < COUNT-1)
			{
				writeChar(row + option, col - 2, "　", F_RED);
				option++;
				writeChar(row + option, col - 2, "→", F_RED | B_H_WHITE);
			}
			break;

		case VK_RETURN:
			clearConsle();
			return option;
			break;
		}
	}
}

//鼠标监听
byte getMousePos(COORD &pos)
{
	INPUT_RECORD inRec;        //定义输入事件结构体  
	DWORD res;      //定义返回记录  
	SetConsoleMode(g_hStdIn, ENABLE_MOUSE_INPUT);
	while (true)
	{
		ReadConsoleInput(g_hStdIn, &inRec, 1, &res);
		if (inRec.EventType == MOUSE_EVENT)
		{
			pos = inRec.Event.MouseEvent.dwMousePosition;
			pos.X /= 2;
			if (inRec.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
				return 1; //左键按下
			else if (inRec.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
				return 2; //右键按下
			return 0;
		}
	}
} //获取鼠标坐标

//选项列表
void showChoose(int top, int edge, int count, char **chooseArr, WORD color, int choose)
{
	if (choose <0 || choose > count - 1)
	{
		return;
	}
	writeChar(top-1, edge, "-------------------------------------------------", F_RED | B_GREEN);
	int i;
	for ( i = 0; i < count; i++)
	{
		if (i == choose)
		{
			writeChar(top + i, edge, chooseArr[i], color | B_RED);
			continue;
		}
		writeChar(top + i, edge, chooseArr[i], color);
	}
	writeChar(top+i, edge, "-------------------------------------------------", F_RED | B_GREEN);
}

//显示消息，0为底部，>0为左边
void showMessage(char *message, WORD color, unsigned int falg)
{
	if (falg == BOTTOM)
	{
		writeChar(WINDOWS_HIGH - 1, WINDOWS_WIDTH / 2 - 20, "                                                      ", color);
		writeChar(WINDOWS_HIGH - 1, WINDOWS_WIDTH / 2 - 20, message, color);
	}
	else
	{
		writeChar(falg, WINDOWS_WIDTH - 24, "                                              ", color);
		writeChar(falg, WINDOWS_WIDTH - 24, message, color);
	}
		
}//弹出通知

//显示对话框
void showDialog(char *str) 
{
	COORD pos;
	int row = (WINDOWS_HIGH - 10);
	int col = (WINDOWS_WIDTH - 20);
	//输出暂停界面
	writeChar(row + 0, col,  "=========================", F_RED | B_WHITE);
	writeChar(row + 1, col,  "                         ", F_RED | B_WHITE);
	writeChar(row + 2, col,/*"     游  戏   暂  停     "*/str, F_RED | B_WHITE);
	writeChar(row + 3, col,  "                         ", F_RED | B_WHITE);
	writeChar(row + 4, col,  "=========点击确认========", F_RED | B_WHITE);
	while (true)
	{
		if (getMousePos(pos) == 1)
		{
			if (pos.X >= col && pos.X <= col + 12 && pos.Y >= row && pos.Y <= row + 4)
				break;
		}
	}
	writeChar(row + 0, col, "                         ", F_RED );
	writeChar(row + 1, col, "                         ", F_RED );
	writeChar(row + 2, col, "                         ", F_RED );
	writeChar(row + 3, col, "                         ", F_RED );
	writeChar(row + 4, col, "                         ", F_RED );
}

//显示文件框
int showFileMenu(char *dir, char *buf, size_t size)
{
	//文件存储信息结构体 
	struct _finddata_t fileinfo;
	//保存文件句柄 
	long fHandle;
	int row ;
	int col ;
	int count = 0, opt = 0;
	char list[FILE_MAX][20];

	if ((fHandle = _findfirst(dir, &fileinfo)) == -1L)
	{
		showMessage("没有发现可以读取的地图", F_H_WHITE, 0);
		return -1;
	}
	else
	{
		clearConsle();
		row = (WINDOWS_HIGH - 5) / 2;
		col = (WINDOWS_WIDTH - strlen("----------") / 2) / 2;
		writeChar(row - 1, col - 2, "---------------", F_RED | B_GREEN);
		do{
			if (count < FILE_MAX)
			{
				writeChar(row + count, col, fileinfo.name, F_WHITE);
				memcpy_s(list[count], 20, fileinfo.name, 20);
				count++;
			}
			else
			{
				break;
			}
		} while (_findnext(fHandle, &fileinfo) == 0);
		_findclose(fHandle); 
		writeChar(row + opt, col - 2, "→", F_RED | B_H_WHITE);
		writeChar(row + count, col - 2, "---------------", F_RED | B_GREEN);
		while (true)
		{
			switch (getKeyCode())
			{
			case VK_UP:
				if (opt >  0)
				{
					writeChar(row + opt, col - 2, "　", F_RED);
					opt--;
					writeChar(row + opt, col - 2, "→", F_RED | B_H_WHITE);
				}
				break;
			case VK_DOWN:
				if (opt < count - 1)
				{
					writeChar(row + opt, col - 2, "　", F_RED);
					opt++;
					writeChar(row + opt, col - 2, "→", F_RED | B_H_WHITE);
				}
				break;

			case VK_RETURN:
				clearConsle();
				strcpy_s(buf, size, list[opt]);
			
				return 0;
			}
		}
	}
}

//添加音乐
void Muisc(char *muiscName)
{
	PlaySoundA(muiscName, NULL, SND_ASYNC | SND_NODEFAULT);
}