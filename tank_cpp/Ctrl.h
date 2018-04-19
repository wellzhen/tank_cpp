#pragma once
#include <sstream>
#include "Data.h"
#include "Maps.h"
#include "Tank.h"
#include "Bullet.h"

class CCtrl
{
public:
	CCtrl();
	~CCtrl();
public:
	//��ʼ��̹����Ϸ
	void playTank(int gameType);

	//�����ת�����
	int dirKey2DirNum(char keyWord);

	//���ÿ���̨��С
	void initConsoleWindow();

	//��ʼ�����뷨
	void initInputShift();

	//��ʾ��ӭǽ�� ��ǽ
	void  showWelcomeWall();
	//��ӭ��
	void  showWelcomeWords();
	//��ʾ��ҳ�˵�
	void showMenu();
	//��ȡ�û�����Ϸ���˵�ѡ��
	int getMenuChoice();
	//��ʾѡ����������˵�
	void showPlayerCountMenu();
	//��ȡ�û����������
	int getPlayerCount();
	//��ʾ����ȡ���еĹؿ��˵� maps������ֻ��
	int chooseLevelPassMenu(CMaps& maps);
	//�浵
	void saveArchive(CMaps& maps, CTank& tanks, CBullet& bullets);
	//����
	void readArchive(CMaps& maps, CTank& tanks, CBullet& bullets);
	//���ȫ�ֱ���
	void clearGlobalData(CMaps& maps, CTank& tanks, CBullet& bullets);
	//�Ҳ���ʾ��Ϸ��Ϣ
	void showGameInfo(CTank& tanks, CBullet& bullets);


};

