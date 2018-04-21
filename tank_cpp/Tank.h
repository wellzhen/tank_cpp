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
	//��ʼ��Npc̹��
	void initNpcTank(int count, int nLevel = 0);

	//��ʼ�����̹��
	void initPlayerTank(int count,int nLevel= 0 );
	//�Զ�����Npc̹��
	void autoRunNpcTank(CBullet& bullets);
	
	//��ʾ̹�� true|| false
	void drawTank(int index, bool isShow);
	void initDrawAllTank(bool isShow);
	//�ƶ�̹��, ������ʾ������
	bool moveTank(int nDir, int index);
	//�ж�Ѫֵ�ʹ� ���е�̹��
	int judgeAlive();
	//��ʾ̹�˵���Ϸ��Ϣ
	void showTankInfo();
	//��ȡ��ȫ�ĳ�����
	void getSafedPoint(int tankIndex, int& posX, int&posY);
	//�ص�������
	void helpPlayer();
private:
	void __initTankShapeModel();
public:
	vector<TANK *>  m_vecTank;
	int m_tankShape[4][3][3]; // TankShape[4][3][3]
	CMaps * m_pMaps;
	int m_nScore;
	int m_nLevel;
};

