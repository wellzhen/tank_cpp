#pragma once
#include <sstream>
#include "Data.h"
class CMaps
{
public:
	//初始化外墙
	void initOuterWall();
	//总部
	void initHeart();
	//初始化地图静态数据
	void initStaticMapData();
	//自定义地图
	void customMapData();
	//显示自定义地图需要的物体
	void showNeedStaticObj();
	//打印地图静态物体
	void drawMap();
	//恢复受损的植物
	void recoverDamagedPlant();
	//读取通过地图文件
	void readStaticMapFile(int fileNum);
	//画静态物体点： 依据m_nMap
	void reDrawMapPoint(int row, int col);
	//打印一个字符  
	void static printChar(int posX, int posY, char* pszCha, WORD wArr);
public:
	CMaps();
	~CMaps();
public:
	int m_nMap[MAPHEIGHT][MAPWIDTH];
	TANK * m_pTankMap[MAPHEIGHT][MAPWIDTH];
	BULLET *  m_pBulletMap[MAPHEIGHT][MAPWIDTH];
	vector<POS> m_vecDamagedPlant;
	bool m_isHeartBroken;
};

