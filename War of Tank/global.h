#pragma once
class Engine;

typedef unsigned char byte;
extern Engine *g_Game;
#define KEY_DOWN(key) GetAsyncKeyState(key)&0x8000  ?1:0	
#define PID  0x1