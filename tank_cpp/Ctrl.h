#pragma once
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

	void shootBullet(int nTankIndex);

	//��ʼ�����뷨
	void initInputShift();

	//��ʾ��ӭǽ
	void  showWelcomeWall();
	//��ӭ��
	void  showWelcomeWords();
	//��ʾ��ҳ�˵�
	void showMenu();
	//��ȡ�û��Ĳ˵�ѡ��
	int getMenuChoice();
	//�浵
	void saveArchive();
	//����
	void readArchive();
	//���ȫ�ֱ���
	void clearGlobalData();
};

