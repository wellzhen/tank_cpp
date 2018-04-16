#include "Ctrl.h"


CCtrl::CCtrl()
{
}


CCtrl::~CCtrl()
{
}

void CCtrl::playTank(int gameType)
{
	//玩家数量
	system("cls");
	showPlayerCountMenu();
	int playerCount = getPlayerCount();

	//开始布置游戏数据
	system("cls");
	CMaps maps;
	CTank tanks(&maps);
	CBullet bullets(&maps);
	//清理全局数据（push_back的数据）
	clearGlobalData(maps, tanks, bullets);
	

	if (gameType == 1) { //经典游戏
		maps.initMapData();
		maps.drawMap();
		//初始化玩家坦克: 默认一个
		tanks.initTank();
		tanks.drawTank(0, true);
		//初始化NPC坦克
		tanks.initNpcTank();
		tanks.drawTank(1, true); // delete? 或者删除初始化中的drawTank
	}
	else if (gameType == 2) {//自定义地图
		maps.showNeedStaticObj();
		maps.initOuterWall();
		maps.drawMap();
		maps.customMapData();

		//初始化玩家坦克: 默认一个
		tanks.initTank();
		tanks.drawTank(0, true);
		//初始化NPC坦克
		tanks.initNpcTank();
		tanks.drawTank(1, true);
	}
	else if (gameType == 3) {//读档
		readArchive(maps, tanks, bullets);
		maps.drawMap();

		//重新画一遍玩家坦克： 会自动更新g_pTankMap
		for (unsigned int i = 0; i < tanks.m_vecTank.size(); i++) {
			if ((!tanks.m_vecTank[i]->isNPC) && tanks.m_vecTank[i]->isAlive) {
				tanks.drawTank(i, true);
			}
		}
	}


	//控制坦克
	char chKey;
	int  nDirNum;
	bool hasPause = false;
	while (1) {
		if (_kbhit()) {
			chKey = _getch();
			if (chKey == 27) {//ESC键
				saveArchive(maps, tanks, bullets);
				return;
			}
			else if (chKey == 32) { //空格
				hasPause = !hasPause; //取反
				continue;
			}
			else if (hasPause) {
				continue;
			}
			else if (chKey == 'j') {//发射炮弹
				bullets.shootBullet(tanks.m_vecTank, 0);//index = 0
			}
			else if (chKey == 'w' || chKey == 'a' || chKey == 's' || chKey == 'd') { //移动按键
				nDirNum = dirKey2DirNum(chKey);
				tanks.moveTank(nDirNum, 0);//暂时第一辆坦克
			}
			else {
				printf("非控制键： %c %d %d\n", chKey, chKey, (int)chKey);
			}
		}
		else if (hasPause) {
			continue;
		}



		//Npc坦克自动运行
		tanks.autoRunNpcTank(bullets);
		//子弹前进
		bullets.runBullet();
		//判断坦克生存情况
		tanks.judgeAlive();
		maps.recoverDamagedPlant();


	}

}

void CCtrl::showPlayerCountMenu()
{

	char menu[][30] = { "1  单人游戏  ",
		"2  双人游戏"
	};

	char tips[] = "请输入您的选择 \n";

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
		printf("尚未设置的按键映射： dirkey2DirNum\n");
		return -1;
		break;
	}
}

void  CCtrl::showWelcomeWall()
{
	for (int row = 0; row < MAPHEIGHT; row++) {
		for (int col = 0; col < MAPWIDTH; col++) {
			if (row == 0 || row == MAPHEIGHT - 1 || col == 0 || col == MAPWIDTH - 1) {
				CMaps::printChar(col, row, "■", COLOR_GRAY);
			}
		}
	}
}
void  CCtrl::showWelcomeWords()
{
	char title[] = "坦克大战";
	char tip[] = "Wlecome To War Of Tank";
	CMaps::printChar((MAPWIDTH - 2 - strlen(title)) / 2, MAPHEIGHT / 2, title, COLOR_WHITE);
	CMaps::printChar((MAPWIDTH - 2 - strlen(tip)) / 2, MAPHEIGHT / 2 + 2, tip, COLOR_RED);
}

void CCtrl::showMenu()
{

	char menu[][30] = { "1  经典游戏  ",
		"2  自定义地图",
		"3  读档游戏  ",
		"4  退出游戏  "
	};

	char tips[] = "请输入您的选择 \n";

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
	//一、地图g_nMap数据
	fwrite(maps.m_nMap, sizeof(int), MAPWIDTH* MAPHEIGHT, pFile);
	//二、坦克
	//坦克数量
	int nTankSize = tanks.m_vecTank.size();
	fwrite(&nTankSize, sizeof(int), 1, pFile);
	//坦克数据
	for (int i = 0; i < nTankSize; i++) {
		fwrite(tanks.m_vecTank[i], sizeof(TANK), 1, pFile);
	}

	//三、子弹
	int nBulletSize = bullets.m_vecBullet.size();
	fwrite(&nBulletSize, sizeof(int), 1, pFile);
	for (int i = 0; i < nBulletSize; i++) {
		fwrite(bullets.m_vecBullet[i], sizeof(BULLET), 1, pFile);
	}
	//五、受损植物地图
	//数量
	int nDamagedPlantSize = maps.m_vecDamagedPlant.size();
	fwrite(&nDamagedPlantSize, sizeof(int), 1, pFile);
	//数据
	for (int i = 0; i < nDamagedPlantSize; i++) {
		fwrite(&(maps.m_vecDamagedPlant[i]), sizeof(POS), 1, pFile);
	}

	fclose(pFile);
}

void CCtrl::readArchive(CMaps& maps, CTank& tanks, CBullet& bullets)
{

	FILE * pFile;
	fopen_s(&pFile, "archive.db", "rb");
	//一、地图g_nMap数据
	fread_s(maps.m_nMap, sizeof(int)*MAPWIDTH*MAPHEIGHT, sizeof(int), MAPWIDTH*MAPHEIGHT, pFile);	//fwrite(g_nMap, sizeof(int), MAPWIDTH* MAPHEIGHT, pFile);
																								//二、坦克
																								//坦克数量
	int nTankSize;
	fread_s(&nTankSize, sizeof(int), sizeof(int), 1, pFile);//fwrite(&nTankSize, sizeof(int), 1, pFile);
															//坦克数据
	for (int i = 0; i < nTankSize; i++) {
		TANK* pTank = (TANK*)malloc(sizeof(TANK));
		fread_s(pTank, sizeof(TANK), sizeof(TANK), 1, pFile);  //fwrite(g_vecTank[i], sizeof(TANK), 1, pFile);
		tanks.m_vecTank.push_back(pTank);
		pTank = NULL;
	}

	//三、子弹
	int nBulletSize;
	fread_s(&nBulletSize, sizeof(int), sizeof(int), 1, pFile);
	for (int i = 0; i < nBulletSize; i++) {
		BULLET* pBullet = (BULLET*)malloc(sizeof(BULLET));
		fread_s(pBullet, sizeof(BULLET), sizeof(BULLET), 1, pFile);//fwrite(g_vecBullet[i], sizeof(BULLET), 1, pFile);
		bullets.m_vecBullet.push_back(pBullet);
		pBullet = NULL;
	}
	//五、受损植物地图
	//数量
	int nDamagedPlantSize;
	fread_s(&nDamagedPlantSize, sizeof(int), sizeof(int), 1, pFile);//fwrite(&nDamagedPlantSize, sizeof(int), 1, pFile);
																	//数据
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
