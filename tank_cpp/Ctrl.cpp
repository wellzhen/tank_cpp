#include "Ctrl.h"


CCtrl::CCtrl()
{
}


CCtrl::~CCtrl()
{
}

void CCtrl::playTank(int gameType)
{
	int playerCount = 0;
	if (gameType != 3 && gameType!= 2) { //非读档和编辑地图
		playerCount = getPlayerCount();//显示UI，获取玩家数量
	}
	if (gameType != 3 && gameType != 2&& playerCount == 0){ //debug
		printf("debug: 玩家数量错误---\n");
		Sleep(5000);
	}
	//开始布置游戏数据
	system("cls");
	CMaps maps;
	CTank tanks(&maps);
	CBullet bullets(&maps);
	//清理全局数据（push_back的数据）-- need ?
	//clearGlobalData(maps, tanks, bullets);
	if (gameType == 1) { //通关游戏
		//maps.initStaticMapData(); //改为读取关卡文件
		int nChoosedMapFile = chooseLevelPassMenu();//显示关卡菜单并选择
		if (nChoosedMapFile == 0) { //返回主菜单
			return;
		}
		else {//读取地图文件，获取地图信息
			maps.readStaticMapFile(nChoosedMapFile);
		}
		maps.drawMap();
		//初始化玩家坦克,并显示:
		tanks.initPlayerTank(playerCount);
		//初始化NPC坦克， 并显示
		tanks.initNpcTank(3);
	}
	else if (gameType == 2) {//编辑地图： 坦克是占位用的
		maps.showNeedStaticObj();
		maps.initOuterWall();
		maps.initHeart();
		maps.drawMap();
		//初始化玩家坦克
		tanks.initPlayerTank(2);
		//初始化NPC坦克
		tanks.initNpcTank(18);
		//显示坦克
		tanks.initDrawAllTank(true);

		maps.customMapData();

		return; //change：直接返回界面
		
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

	/*********************操作坦克*******************/
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
			else if (chKey == 'f') {//发射炮弹
				bullets.shootBullet(tanks.m_vecTank, 0);//index = 0
			}
			else if (chKey == 'h' && playerCount == 2) {//第二玩家
				bullets.shootBullet(tanks.m_vecTank, 1);
			}
			else if (chKey == 'w' || chKey == 'a' || chKey == 's' || chKey == 'd') { //移动按键
				nDirNum = dirKey2DirNum(chKey);
				tanks.moveTank(nDirNum, 0);//暂时第一辆坦克
			}
			else if (playerCount == 2 &&( chKey == 'j' || chKey == 'k' || chKey == 'i' || chKey == 'l')) { //移动按键
				nDirNum = dirKey2DirNum(chKey);
				tanks.moveTank(nDirNum, 1);//暂时第2辆坦克
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
		//判断心脏
		if (maps.m_isHeartBroken) { //失败
			Sleep(2000);
			return;
		}
		//判断坦克生存情况
		int tankResult = tanks.judgeAlive();
		//显示战斗信息
		showGameInfo(tanks, bullets);

		if (tankResult == -1) { //失败
			Sleep(2000);
			return;
		}
		else if (tankResult == 1) { //胜利
			Sleep(2000);
			return;
		}
		maps.recoverDamagedPlant();
	}

}

int CCtrl::chooseLevelPassMenu()
{
	vector<int>  vecPassLevel;//保存有的关卡
	for (int i = 1; i < 38; i++) {
		int nFileNum = i;
		stringstream stream;
		stream << nFileNum;
		string  strFile = stream.str();
		strFile.insert(0, "map\\"); //拼凑文件路径
		const char * pFilename = strFile.c_str();
		FILE * pFile;
		errno_t errNum = fopen_s(&pFile, pFilename, "rb");

		if (errNum == 0) {//文件存在
			vecPassLevel.push_back(i);
			fclose(pFile);
		}
	}
	//确定显示的起始行坐标row
	int startPosX = int((MAPWIDTH - 8) / 2);
	int startPosY = int((MAPHEIGHT - vecPassLevel.size())/2);
	//打印界面
	system("cls");
	for ( int i = 0; i < (int)vecPassLevel.size(); i++) {
		int nFileNum = vecPassLevel[i];
		stringstream stream;
		stream << nFileNum;
		string  strFile = stream.str();
		char * pNum = (char *)strFile.c_str();
		CMaps::printChar(startPosX - 6, startPosY + i, "关卡 【 ", COLOR_GRAY);
		CMaps::printChar(startPosX, startPosY + i, pNum, COLOR_GRAY);
		CMaps::printChar(startPosX + 4, startPosY + i, "  】", COLOR_GRAY);
	}
	CMaps::printChar(startPosX, startPosY + vecPassLevel.size(), "【返回】", COLOR_RED);//返回

	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD stcRecord = { 0 };
	DWORD dwRead;
	SetConsoleMode(hStdin, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
	int mousePosX = 0;
	int mousePosY = 0;
	while (1)
	{
		ReadConsoleInput(hStdin, &stcRecord, 1, &dwRead);
		if (stcRecord.EventType == MOUSE_EVENT) {
			MOUSE_EVENT_RECORD mer = stcRecord.Event.MouseEvent;
			switch (mer.dwEventFlags)
			{
			case 0://单击
				if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) { //左键单击
					if (mousePosX > startPosX - 10 && mousePosX < startPosX + 20 ) //限定鼠标左右范围
						if (mousePosY >= startPosY && mousePosY <= startPosY + (int)vecPassLevel.size() - 1) {    //限定鼠标点击的Y方向范围
							int vecIndex = mousePosY - startPosY;
							return vecPassLevel[vecIndex];
						} else if(mousePosY == startPosY + (int)vecPassLevel.size()) {
							return 0;
					}
				}
				break;
			case MOUSE_MOVED:
				mousePosX = mer.dwMousePosition.X / 2;
				mousePosY = mer.dwMousePosition.Y;
				break;
			default:
				break;
			}
		}
	}


}

void CCtrl::showPlayerCountMenu()
{

	char menu[][30] = { "1  单人游戏  ", 
						"2  双人游戏  " 
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
	int playerCount = 0;
	while (playerCount != 1 && playerCount != 2) {
		system("cls");
		showPlayerCountMenu();
		scanf_s("%d", &playerCount);
	}

	return playerCount;

}

void CCtrl::initConsoleWindow()
{
	system("mode con cols=145 lines=42");
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
	case 'i':
		return DIR_UP;
		break;
	case 'a':
	case 'j':
		return DIR_LEFT;
		break;
	case 's':
	case 'k':
		return DIR_DOWN;
		break;
	case 'd':
	case 'l':
		return DIR_RIGHT;
		break;
	default:
		printf("debug: 暂无功能的按键： dirkey2DirNum\n");
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

	char menu[][30] = { 
		"1  闯关游戏  ",
		"2  编辑地图  ",
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
	while (menuNum != 1 && menuNum != 2 && menuNum != 3 && menuNum != 4) {
		system("cls");
		showWelcomeWall();
		showMenu();
		scanf_s("%d", &menuNum);
		int ch;
		while ((ch = getchar()) != EOF && ch != '\n');
	}
	
	return menuNum;
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



void CCtrl::showGameInfo(CTank& tanks, CBullet& bullets)
{
	tanks.showTankInfo();

}