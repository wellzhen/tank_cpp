#pragma once
#include "Data.h"
class CBullet
{
public:
	CBullet();
	~CBullet();
public:
	//�����ӵ�
	void shootBullet(int nTankIndex);
	//�ڵ����� 
	bool runBullet();
	//�����ӵ�
	void drawBullet(bool isShow);
	//�Ƴ���Ч���ӵ�
	void removeInvalidBullet();
private:
	vector<BULLET *> m_vecBullet;
};

