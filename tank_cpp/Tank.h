#pragma once
#include "Data.h"
class CTank
{
public:
	CTank();
	~CTank();
public:
	//��ʼ��Npc̹��
	void initNpcTank();

	//��ʼ�����̹��
	void initTank();
	//�Զ�����Npc̹��
	void autoRunNpcTank();

	//��ʾ̹�� true|| false
	void drawTank(int index, bool isShow);

	//�ƶ�̹��, ������ʾ������
	bool moveTank(int nDir, int index);

	//�ж�Ѫֵ�ʹ� ���е�̹��
	void judgeAlive();
private:
	vector<TANK *>  m_vecTank;
};

