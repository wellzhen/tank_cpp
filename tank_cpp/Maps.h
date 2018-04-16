#pragma once
#include "Data.h"
class CMaps
{
public:
	//��ʼ����ǽ
	void initOuterWall();
	//��ʼ����ͼ
	void initMapData();
	//�Զ����ͼ
	void customMapData();
	//��ʾ�Զ����ͼ��Ҫ������
	void showNeedStaticObj();
	//��ӡ��ͼ��̬����
	void drawMap();
	//�ָ������ֲ��
	void recoverDamagedPlant();
	//����̬����㣺 ����m_nMap
	void reDrawMapPoint(int row, int col);
	//��ӡһ���ַ�  
	void static printChar(int posX, int posY, char* pszCha, WORD wArr);
public:
	CMaps();
	~CMaps();
public:
	int m_nMap[MAPHEIGHT][MAPWIDTH];
	TANK * m_pTankMap[MAPHEIGHT][MAPWIDTH];
	BULLET *  m_pBulletMap[MAPHEIGHT][MAPWIDTH];
	vector<POS> m_vecDamagedPlant;
};

