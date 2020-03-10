#pragma once
#include "Missile.h"
#define RID 4
class Player;

class RBase
	:public Missile
{
public:
	RBase(int style);
	virtual bool run(Map *map, Player *play )= 0;
	~RBase();
};

