#include "Maps.h"

CMaps::CMaps()
{
	int m_nMap[MAPHEIGHT][MAPWIDTH] = { 0 };
	TANK * m_pTankMap[MAPHEIGHT][MAPWIDTH] = { NULL };
	BULLET *  m_pBulletMap[MAPHEIGHT][MAPWIDTH] = { NULL };
}

CMaps::~CMaps()
{
}

void CMaps::initOuterWall()
{
	for (int row = 0; row < MAPHEIGHT; row++) {
		for (int col = 0; col < MAPWIDTH; col++) {
			if (row == 0 || row == MAPHEIGHT - 1 || col == 0 || col == MAPWIDTH - 1) {
				m_nMap[row][col] = MAP_STONE;
			}
			else {
				m_nMap[row][col] = MAP_SPACE;
			}
		}
	}
}

void CMaps::initMapData()
{
	initOuterWall();
	for (int col = 1; col < MAPWIDTH - 1; col++) {
		m_nMap[4][col] = MAP_GRASS;
		m_nMap[5][col] = MAP_GRASS;
	}

	for (int col = 1; col < MAPWIDTH - 1; col++) {
		m_nMap[8][col] = MAP_TREE;
		m_nMap[9][col] = MAP_TREE;
	}

	for (int col = 1; col < MAPWIDTH - 1; col++) {
		m_nMap[12][col] = MAP_ICE;
		m_nMap[13][col] = MAP_ICE;
	}


	for (int col = 0; col < 10; col++) {
		m_nMap[17][col] = MAP_STONE;
		m_nMap[18][col] = MAP_STONE;
	}

	for (int col = MAPWIDTH - 15; col < MAPWIDTH - 1; col++) {
		m_nMap[17][col] = MAP_RIVER;
		m_nMap[18][col] = MAP_RIVER;
	}
	for (int row = 20; row < 30; row++) {
		for (int col = 1; col < MAPWIDTH - 1; col++) {
			m_nMap[row][col] = MAP_BRICK;

		}
	}

}

void CMaps::showNeedStaticObj()
{
	int nDestPosX = MAPWIDTH + 2;
	printChar(nDestPosX, 2, "�� �յ�", COLOR_WHITE); //�յ�
	printChar(nDestPosX, 4, "�� ʯ��", COLOR_GRAY);  //ʯ��
	printChar(nDestPosX, 6, "�� ש��", COLOR_RED_LIGHT); // ש��
	printChar(nDestPosX, 8, "�� �ݵ�", COLOR_GREEN);   //��
	printChar(nDestPosX, 10, "�� ѩ��", COLOR_WHITE);   //ѩ
	printChar(nDestPosX, 12, "�� ��ľ", COLOR_GREEN_LIGHT); //��
	printChar(nDestPosX, 14, "�� ����", COLOR_GRAY);   //����

	printChar(nDestPosX, 18, "Tip ����༭", COLOR_GRAY);
	printChar(nDestPosX, 20, "    ˫��ɾ��", COLOR_GRAY);
	printChar(nDestPosX, 22, "    �Ҽ���Ϸ", COLOR_GRAY);
}


void CMaps::customMapData()
{
	initOuterWall();
	int nColorBoxPosX = MAPWIDTH + 1; //ͬshowNeedStaticObj()����һ��

	int nBrushColorNum = 0; //����ˢ����ɫ
	int mousePosX = 0;
	int mousePosY = 0;

	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD stcRecord = { 0 };
	DWORD dwRead;
	SetConsoleMode(hStdin, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);

	while (1)
	{
		ReadConsoleInput(hStdin, &stcRecord, 1, &dwRead);
		if (stcRecord.EventType == MOUSE_EVENT) {
			MOUSE_EVENT_RECORD mer = stcRecord.Event.MouseEvent;
			switch (mer.dwEventFlags)
			{
			case 0:
				if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
					//�������
					if (mousePosX >= nColorBoxPosX) { //�������������
						switch (mousePosY) {
						case 2:
							nBrushColorNum = MAP_SPACE;
							break;
						case 4:
							nBrushColorNum = MAP_STONE;
							break;
						case 6:
							nBrushColorNum = MAP_BRICK;
							break;
						case 8:
							nBrushColorNum = MAP_GRASS;
							break;
						case 10:
							nBrushColorNum = MAP_ICE;
							break;
						case 12:
							nBrushColorNum = MAP_TREE;
							break;
						case 14:
							nBrushColorNum = MAP_RIVER;
							break;
						default:
							//
							break;
						}
					}
					else if (mousePosX > 0 && mousePosX < MAPWIDTH - 1 && mousePosY > 0 && mousePosY < MAPHEIGHT - 1) {
						m_nMap[mousePosY][mousePosX] = nBrushColorNum;
						reDrawMapPoint(mousePosY, mousePosX);
					}
				}
				else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
				{
					//�Ҽ�����: �����༭�༭
					return;
				}
				break;
			case DOUBLE_CLICK:
				//˫�� 
				if (mousePosX > 0 && mousePosX < MAPWIDTH - 1 && mousePosY > 0 && mousePosY < MAPHEIGHT - 1) {
					m_nMap[mousePosY][mousePosX] = 0;
					reDrawMapPoint(mousePosY, mousePosX);
				}
				break;
			case MOUSE_MOVED:
				mousePosX = mer.dwMousePosition.X / 2;
				mousePosY = mer.dwMousePosition.Y;
				break;
			}
		}
	}

}

//������ posY,posX��
void CMaps::reDrawMapPoint(int row, int col)
{
	if (m_nMap[row][col] == MAP_STONE) {
		printChar(col, row, "��", COLOR_GRAY);
	}
	else if (m_nMap[row][col] == MAP_BRICK) {
		printChar(col, row, "��", COLOR_RED_LIGHT);
	}
	else if (m_nMap[row][col] == MAP_SPACE) {
		printChar(col, row, "  ", COLOR_BLACK);
	}
	else if (m_nMap[row][col] == MAP_GRASS) {
		printChar(col, row, "��", COLOR_GREEN);
	}
	else if (m_nMap[row][col] == MAP_ICE) {
		printChar(col, row, "��", COLOR_WHITE);
	}
	else if (m_nMap[row][col] == MAP_TREE) {
		printChar(col, row, "��", COLOR_GREEN_LIGHT);
	}
	else if (m_nMap[row][col] == MAP_RIVER) {
		printChar(col, row, "��", COLOR_GRAY);
	}
	else {
		printf("δ����ĵ�ͼ����\n");
	}
}

void CMaps::drawMap()
{
	for (int row = 0; row < MAPHEIGHT; row++) {
		for (int col = 0; col < MAPWIDTH; col++) {
			reDrawMapPoint(row, col);
		}
	}

}

void CMaps::recoverDamagedPlant()
{
	for (vector<POS>::iterator it = m_vecDamagedPlant.begin(); it != m_vecDamagedPlant.end(); ) {
		int posX = it->posX;
		int posY = it->posY;
		if (m_nMap[posY][posX] != 0 && m_pBulletMap[posY][posX] == NULL && m_pTankMap[posY][posX] == NULL) {
			//û��̹�˻��ӵ�ʱ�Żָ�ֲ��
			reDrawMapPoint(posY, posX);
			it = m_vecDamagedPlant.erase(it);
		}
		else {
			++it;
		}
	}

}

void CMaps::printChar(int posX, int posY, char* pszChar, WORD wArr)
{
	//1 ���ù������
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 1;
	cci.bVisible = false;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
	//2. �޸Ĺ���ڿ���̨�е�����λ��
	COORD loc;
	loc.X = posX * 2;
	loc.Y = posY;
	// ������ɫ
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wArr);
	//3. ���ù���ڿ���̨�е�����λ��
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), loc);
	printf(pszChar);
}


