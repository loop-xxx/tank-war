#pragma once
#include "RBase.h"
class RMiss3 :
	public RBase
{
public:
	RMiss3();

	bool run(Map *map, Player *play);
	bool checkMissile();
	int DrawMissile(Map *map);
	void ClsMissile(Map *map);

	~RMiss3();
};

