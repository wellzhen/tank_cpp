#pragma once
#include "Data.h"
#include "Maps.h"

class CTank;

class CBullet
{
public:
	CBullet(CMaps* pMaps);
	~CBullet();
public:
	//�����ӵ�
	void shootBullet(vector<TANK *>& m_vecTank, int nTankIndex);
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

