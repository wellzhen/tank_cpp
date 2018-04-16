#include "Ctrl.h"


CCtrl::CCtrl()
{
}


CCtrl::~CCtrl()
{
}

void CCtrl::playTank(int gameType)
{
	//�������
	system("cls");
	showPlayerCountMenu();
	int playerCount = getPlayerCount();

	//��ʼ������Ϸ����
	system("cls");
	CMaps maps;
	CTank tanks(&maps);
	CBullet bullets(&maps);
	//����ȫ�����ݣ�push_back�����ݣ�
	clearGlobalData(maps, tanks, bullets);
	

	if (gameType == 1) { //������Ϸ
		maps.initMapData();
		maps.drawMap();
		//��ʼ�����̹��: Ĭ��һ��
		tanks.initTank();
		tanks.drawTank(0, true);
		//��ʼ��NPC̹��
		tanks.initNpcTank();
		tanks.drawTank(1, true); // delete? ����ɾ����ʼ���е�drawTank
	}
	else if (gameType == 2) {//�Զ����ͼ
		maps.showNeedStaticObj();
		maps.initOuterWall();
		maps.drawMap();
		maps.customMapData();

		//��ʼ�����̹��: Ĭ��һ��
		tanks.initTank();
		tanks.drawTank(0, true);
		//��ʼ��NPC̹��
		tanks.initNpcTank();
		tanks.drawTank(1, true);
	}
	else if (gameType == 3) {//����
		readArchive(maps, tanks, bullets);
		maps.drawMap();

		//���»�һ�����̹�ˣ� ���Զ�����g_pTankMap
		for (unsigned int i = 0; i < tanks.m_vecTank.size(); i++) {
			if ((!tanks.m_vecTank[i]->isNPC) && tanks.m_vecTank[i]->isAlive) {
				tanks.drawTank(i, true);
			}
		}
	}


	//����̹��
	char chKey;
	int  nDirNum;
	bool hasPause = false;
	while (1) {
		if (_kbhit()) {
			chKey = _getch();
			if (chKey == 27) {//ESC��
				saveArchive(maps, tanks, bullets);
				return;
			}
			else if (chKey == 32) { //�ո�
				hasPause = !hasPause; //ȡ��
				continue;
			}
			else if (hasPause) {
				continue;
			}
			else if (chKey == 'j') {//�����ڵ�
				bullets.shootBullet(tanks.m_vecTank, 0);//index = 0
			}
			else if (chKey == 'w' || chKey == 'a' || chKey == 's' || chKey == 'd') { //�ƶ�����
				nDirNum = dirKey2DirNum(chKey);
				tanks.moveTank(nDirNum, 0);//��ʱ��һ��̹��
			}
			else {
				printf("�ǿ��Ƽ��� %c %d %d\n", chKey, chKey, (int)chKey);
			}
		}
		else if (hasPause) {
			continue;
		}



		//Npc̹���Զ�����
		tanks.autoRunNpcTank(bullets);
		//�ӵ�ǰ��
		bullets.runBullet();
		//�ж�̹���������
		tanks.judgeAlive();
		maps.recoverDamagedPlant();


	}

}

void CCtrl::showPlayerCountMenu()
{

	char menu[][30] = { "1  ������Ϸ  ",
		"2  ˫����Ϸ"
	};

	char tips[] = "����������ѡ�� \n";

	for (int i = 0; i< _countof(menu); i++) {
		CMaps::printChar((MAPWIDTH - 2 - strlen(menu[i])) / 2, MAPHEIGHT / 2 - 6 + i * 2, menu[i], COLOR_WHITE);
	}
	CMaps::printChar((MAPWIDTH - 2 - strlen(tips)) / 2, MAPHEIGHT / 2 - 4 + _countof(menu) * 2, tips, COLOR_RED);
	CMaps::printChar((MAPWIDTH + 2 - strlen(tips)) / 2, MAPHEIGHT / 2 - 2 + _countof(menu) * 2, " ", COLOR_RED);
}

int CCtrl::getPlayerCount()
{
	int menuNum = 0;
	while (1) {
		if (menuNum == 1 || menuNum == 2) {
			return menuNum;
		}
		else {
			//menuNum = 0;
			scanf_s("%d", &menuNum);
		}
	}
}

void CCtrl::initConsoleWindow()
{
	system("mode con cols=140 lines=50");
}

void CCtrl::initInputShift()
{
	keybd_event(VK_SHIFT, 0, 0, 0);
	keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
}

int CCtrl::dirKey2DirNum(char keyWord)
{
	switch (keyWord) {
	case 'w':
		return DIR_UP;
		break;
	case 'a':
		return DIR_LEFT;
		break;
	case 's':
		return DIR_DOWN;
		break;
	case 'd':
		return DIR_RIGHT;
		break;
	default:
		printf("��δ���õİ���ӳ�䣺 dirkey2DirNum\n");
		return -1;
		break;
	}
}

void  CCtrl::showWelcomeWall()
{
	for (int row = 0; row < MAPHEIGHT; row++) {
		for (int col = 0; col < MAPWIDTH; col++) {
			if (row == 0 || row == MAPHEIGHT - 1 || col == 0 || col == MAPWIDTH - 1) {
				CMaps::printChar(col, row, "��", COLOR_GRAY);
			}
		}
	}
}
void  CCtrl::showWelcomeWords()
{
	char title[] = "̹�˴�ս";
	char tip[] = "Wlecome To War Of Tank";
	CMaps::printChar((MAPWIDTH - 2 - strlen(title)) / 2, MAPHEIGHT / 2, title, COLOR_WHITE);
	CMaps::printChar((MAPWIDTH - 2 - strlen(tip)) / 2, MAPHEIGHT / 2 + 2, tip, COLOR_RED);
}

void CCtrl::showMenu()
{

	char menu[][30] = { "1  ������Ϸ  ",
		"2  �Զ����ͼ",
		"3  ������Ϸ  ",
		"4  �˳���Ϸ  "
	};

	char tips[] = "����������ѡ�� \n";

	for (int i = 0; i< _countof(menu); i++) {
		CMaps::printChar((MAPWIDTH - 2 - strlen(menu[i])) / 2, MAPHEIGHT / 2 - 6 + i * 2, menu[i], COLOR_WHITE);
	}
	CMaps::printChar((MAPWIDTH - 2 - strlen(tips)) / 2, MAPHEIGHT / 2 - 4 + _countof(menu) * 2, tips, COLOR_RED);
	CMaps::printChar((MAPWIDTH + 2 - strlen(tips)) / 2, MAPHEIGHT / 2 - 2 + _countof(menu) * 2, " ", COLOR_RED);
}

int CCtrl::getMenuChoice()
{
	int menuNum = 0;
	while (1) {
		if (menuNum == 1 || menuNum == 2 || menuNum == 3 || menuNum == 4 || menuNum == 5 || menuNum == 6) {
			return menuNum;
		}
		else {
			//menuNum = 0;
			scanf_s("%d", &menuNum);
		}
	}
}

void CCtrl::saveArchive(CMaps& maps, CTank& tanks, CBullet& bullets)
{
	FILE * pFile;
	fopen_s(&pFile, "archive.db", "wb");
	//һ����ͼg_nMap����
	fwrite(maps.m_nMap, sizeof(int), MAPWIDTH* MAPHEIGHT, pFile);
	//����̹��
	//̹������
	int nTankSize = tanks.m_vecTank.size();
	fwrite(&nTankSize, sizeof(int), 1, pFile);
	//̹������
	for (int i = 0; i < nTankSize; i++) {
		fwrite(tanks.m_vecTank[i], sizeof(TANK), 1, pFile);
	}

	//�����ӵ�
	int nBulletSize = bullets.m_vecBullet.size();
	fwrite(&nBulletSize, sizeof(int), 1, pFile);
	for (int i = 0; i < nBulletSize; i++) {
		fwrite(bullets.m_vecBullet[i], sizeof(BULLET), 1, pFile);
	}
	//�塢����ֲ���ͼ
	//����
	int nDamagedPlantSize = maps.m_vecDamagedPlant.size();
	fwrite(&nDamagedPlantSize, sizeof(int), 1, pFile);
	//����
	for (int i = 0; i < nDamagedPlantSize; i++) {
		fwrite(&(maps.m_vecDamagedPlant[i]), sizeof(POS), 1, pFile);
	}

	fclose(pFile);
}

void CCtrl::readArchive(CMaps& maps, CTank& tanks, CBullet& bullets)
{

	FILE * pFile;
	fopen_s(&pFile, "archive.db", "rb");
	//һ����ͼg_nMap����
	fread_s(maps.m_nMap, sizeof(int)*MAPWIDTH*MAPHEIGHT, sizeof(int), MAPWIDTH*MAPHEIGHT, pFile);	//fwrite(g_nMap, sizeof(int), MAPWIDTH* MAPHEIGHT, pFile);
																								//����̹��
																								//̹������
	int nTankSize;
	fread_s(&nTankSize, sizeof(int), sizeof(int), 1, pFile);//fwrite(&nTankSize, sizeof(int), 1, pFile);
															//̹������
	for (int i = 0; i < nTankSize; i++) {
		TANK* pTank = (TANK*)malloc(sizeof(TANK));
		fread_s(pTank, sizeof(TANK), sizeof(TANK), 1, pFile);  //fwrite(g_vecTank[i], sizeof(TANK), 1, pFile);
		tanks.m_vecTank.push_back(pTank);
		pTank = NULL;
	}

	//�����ӵ�
	int nBulletSize;
	fread_s(&nBulletSize, sizeof(int), sizeof(int), 1, pFile);
	for (int i = 0; i < nBulletSize; i++) {
		BULLET* pBullet = (BULLET*)malloc(sizeof(BULLET));
		fread_s(pBullet, sizeof(BULLET), sizeof(BULLET), 1, pFile);//fwrite(g_vecBullet[i], sizeof(BULLET), 1, pFile);
		bullets.m_vecBullet.push_back(pBullet);
		pBullet = NULL;
	}
	//�塢����ֲ���ͼ
	//����
	int nDamagedPlantSize;
	fread_s(&nDamagedPlantSize, sizeof(int), sizeof(int), 1, pFile);//fwrite(&nDamagedPlantSize, sizeof(int), 1, pFile);
																	//����
	for (int i = 0; i < nDamagedPlantSize; i++) {
		POS  damagedPlant;
		fread_s(&damagedPlant, sizeof(POS), sizeof(POS), 1, pFile);//fwrite(&g_vecDamagedPlant[i], sizeof(POS), 1, pFile);
		maps.m_vecDamagedPlant.push_back(damagedPlant);
	}
	fclose(pFile);
}

void CCtrl::clearGlobalData(CMaps& maps, CTank& tanks, CBullet& bullets)
{
	tanks.m_vecTank.clear();
	bullets.m_vecBullet.clear();
	maps.m_vecDamagedPlant.clear();
	for (int row = 0; row < MAPHEIGHT; row++) {
		for (int col = 0; col < MAPWIDTH; col++) {
			maps.m_pTankMap[row][col] = 0;
			maps.m_pBulletMap[row][col] = 0;
			maps.m_nMap[row][col] = 0;
		}
	}
}
