#pragma once
#include "Data.h"
class CTank
{
public:
	CTank();
	~CTank();
public:
	//初始化Npc坦克
	void initNpcTank();

	//初始化玩家坦克
	void initTank();
	//自动运行Npc坦克
	void autoRunNpcTank();

	//显示坦克 true|| false
	void drawTank(int index, bool isShow);

	//移动坦克, 包含显示和隐藏
	bool moveTank(int nDir, int index);

	//判断血值和存活： 所有的坦克
	void judgeAlive();
private:
	vector<TANK *>  m_vecTank;
};

