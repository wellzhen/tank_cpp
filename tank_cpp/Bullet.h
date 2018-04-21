#pragma once
#include "Data.h"
#include "Maps.h"
#include "Bgm.h"

class CTank;

class CBullet
{
public:
	CBullet(CMaps* pMaps);
	~CBullet();
public:
	//�����ӵ�
	void shootBullet(vector<TANK *>& m_vecTank, int nTankIndex);
	//�����ӵ��Ƿ���Է�����
	void canShoot(vector<TANK*>&m_vecTank);
	//�ڵ����� 
	bool runBullet();
	//�����ӵ�
	void drawBullet(bool isShow);
	//�Ƴ���Ч���ӵ�
	void removeInvalidBullet();
public:
	vector<BULLET *> m_vecBullet;
	CMaps * m_pMaps;
};

