#pragma once
#include "Data.h"
class CMaps
{
public:
	CMaps();
	~CMaps();
public:
	//初始化外墙
	void initOuterWall();
	//初始化地图
	void initMapData();
	//自定义地图
	void customMapData();
	//显示自定义地图需要的物体
	void showNeedStaticObj();
	//打印地图静态物体
	void drawMap();

	//恢复受损的植物
	void recoverDamagedPlant();

	//画静态物体点： 依据g_nMap
	void reDrawMapPoint(int row, int col);

	//打印一个字符  
	void printChar(int posX, int posY, char* pszCha, WORD wArr);
private:
	int m_nMap[MAPHEIGHT][MAPWIDTH];
	TANK * m_pTankMap[MAPHEIGHT][MAPWIDTH];
	BULLET *  m_pBulletMap[MAPHEIGHT][MAPWIDTH];
	vector<POS> g_vecDamagedPlant;
};

