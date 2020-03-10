#pragma once
#include <time.h>
#include <Windows.h>
#include "data.h"
#include <stdio.h>

extern  void writeChar(int row, int col, const char* pszChar, WORD wArr);
extern void initConsole();
extern byte showMenu(byte COUNT, char**menuArr);
extern void clearConsle();
extern byte getKeyCode(); 
extern byte getMousePos(COORD &pos);
extern void showChoose(int top, int edge, int count, char **chooseArr, WORD color, int choose);
extern int showFileMenu(char *dir, char *buf, size_t size);
extern void showDialog(char *str);
extern void Muisc(char *muiscName);

#define BOTTOM 0
extern void showMessage(char *message, WORD color, unsigned int flag);

