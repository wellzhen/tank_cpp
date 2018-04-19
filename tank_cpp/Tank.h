#pragma once
#include "Data.h"
#include "Maps.h"
#include "Bullet.h"
#include "Astar.h"
class CTank
{
public:
	CTank(CMaps* pMaps);
	~CTank();
public:
	//初始化Npc坦克
	void initNpcTank(int count);

	//初始化玩家坦克
	void initPlayerTank(int count);
	//自动运行Npc坦克
	void autoRunNpcTank(CBullet& bullets);

	//显示坦克 true|| false
	void drawTank(int index, bool isShow);
	void initDrawAllTank(bool isShow);
	//移动坦克, 包含显示和隐藏
	bool moveTank(int nDir, int index);
	//判断血值和存活： 所有的坦克
	int judgeAlive();
	//显示坦克的游戏信息
	void showTankInfo();
private:
	void __initTankShapeModel();
public:
	vector<TANK *>  m_vecTank;
	int m_tankShape[4][3][3]; // TankShape[4][3][3]
	CMaps * m_pMaps;
};

