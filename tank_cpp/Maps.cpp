#include "Maps.h"

CMaps::CMaps()
{
	m_nMap[MAPHEIGHT][MAPWIDTH] = { 0 };
	m_pTankMap[MAPHEIGHT][MAPWIDTH] = { NULL };
	m_pBulletMap[MAPHEIGHT][MAPWIDTH] = { NULL };
	m_isHeartBroken = false;
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

void CMaps::initHeart()
{
	int Y = MAPHEIGHT - 3;
	int X = MAPWIDTH / 2 - 1;
	m_nMap[Y][X] = MAP_HEART;
	//����ǽ
	m_nMap[Y+1][X + 1] = MAP_BRICK;
	m_nMap[Y +1][X - 1] = MAP_BRICK;
	m_nMap[Y][X + 1] = MAP_BRICK;
	m_nMap[Y][X - 1] = MAP_BRICK;
	m_nMap[Y - 1][X] = MAP_BRICK;
	m_nMap[Y - 1][X + 1] = MAP_BRICK;
	m_nMap[Y - 1][X - 1] = MAP_BRICK;

}

void CMaps::initStaticMapData()
{
	initOuterWall();
	initHeart();
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
	printChar(nDestPosX, 0, "��  ���ز˵�  ��", COLOR_RED); //�յ�

	printChar(nDestPosX, 2, "�� �յ�", COLOR_WHITE); //�յ�
	printChar(nDestPosX, 4, "�� ʯ��", COLOR_GRAY);  //ʯ��
	printChar(nDestPosX, 6, "�� ש��", COLOR_RED_LIGHT); // ש��
	printChar(nDestPosX, 8, "�� �ݵ�", COLOR_GREEN);   //��
	printChar(nDestPosX, 10, "�� ѩ��", COLOR_WHITE);   //ѩ
	printChar(nDestPosX, 12, "�� ��ľ", COLOR_GREEN_LIGHT); //��
	printChar(nDestPosX, 14, "�� ����", COLOR_GRAY);   //����


	printChar(nDestPosX, 28, "���������йؿ�ͼ��", COLOR_GRAY);
	printChar(nDestPosX + 3, 29, "��", COLOR_GRAY); printChar(nDestPosX + 4, 29, "  ", COLOR_GRAY); printChar(nDestPosX + 5, 29, "��", COLOR_GRAY);

	printChar(nDestPosX, 30, "������Ϊȱʧ�ؿ���", COLOR_GRAY);
	printChar(nDestPosX , 31, "������:ָ���ؿ��š�", COLOR_GRAY);
	printChar(nDestPosX+3, 32, "��", COLOR_GRAY); printChar(nDestPosX + 4, 32, "  ", COLOR_GRAY); printChar(nDestPosX + 5, 32, "��", COLOR_GRAY);
	

	printChar(nDestPosX, MAPHEIGHT - 3, "Tip �༭", COLOR_GRAY);
	printChar(nDestPosX, MAPHEIGHT - 2, "    ɾ��", COLOR_GRAY);
	printChar(nDestPosX, MAPHEIGHT - 1, "    ��Ϸ", COLOR_GRAY);
}

void CMaps::customMapData()
{
	int nColorBoxPosX = MAPWIDTH + 1; //ͬshowNeedStaticObj()����һ��

	int nBrushColorNum = 0; //����ˢ����ɫ
	int mousePosX = 0;
	int mousePosY = 0;

	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD stcRecord = { 0 };
	DWORD dwRead;
	SetConsoleMode(hStdin, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);


	bool activeScrollPainting = false;
	int nLevelPass = 1;//ǿ��Ҫ����Ĺؿ���
	int nImportLevelPass = -1; //��Ҫ����Ĺؿ���
	while (1)
	{
		ReadConsoleInput(hStdin, &stcRecord, 1, &dwRead);
		if (stcRecord.EventType == MOUSE_EVENT) {
			MOUSE_EVENT_RECORD mer = stcRecord.Event.MouseEvent;
			switch (mer.dwEventFlags)
			{
			case 0://����
				if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {//�������
					
					if (mousePosX >= nColorBoxPosX) { //�������������
						switch (mousePosY) {
						case 0://����
							return;
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
						case 28: //����ؿ�
							break;
						case 29://ѡ����Ĺؿ���
							{
								bool hasFind = false;
								int  loopTimes = 0;
								while(!hasFind ) {
									loopTimes++;
									if (loopTimes > 40) { //�Ҳ����ļ���
										break;
									}
									if (mousePosX == MAPWIDTH + 5) {//��С�ؿ����֣�
										nImportLevelPass--;
										nImportLevelPass = nImportLevelPass >= 1 ? nImportLevelPass : 38;

									}
									else if (mousePosX == MAPWIDTH + 7) { //����ؿ�����
										nImportLevelPass++;
										nImportLevelPass = nImportLevelPass <= 38 ? nImportLevelPass : 1;
									}
									int nFileNum = nImportLevelPass;
									stringstream stream;
									stream << nFileNum;
									string  strFile = stream.str();
									string  strFileCopy = strFile;
									char * strFileNum = (char *)strFileCopy.c_str();  //UI��ʾʹ��
									strFile.insert(0, "map\\"); //ƴ���ļ�·��
									const char * pFilename = strFile.c_str();
									FILE * pFile;
									errno_t errNum = fopen_s(&pFile, pFilename, "rb");

									if (errNum == 0) {//�ļ�����: ��ȡ-������
										hasFind = true;
										fread_s(m_nMap, MAPHEIGHT*MAPWIDTH * sizeof(int), sizeof(int), MAPWIDTH*MAPHEIGHT, pFile);
										fclose(pFile);
										//��ʾ��ǰ�Ĺؿ�����
										printChar(MAPWIDTH + 6, 29, "  ", COLOR_GRAY);//���
										printChar(MAPWIDTH + 6, 29, strFileNum, COLOR_GRAY);
										//�ػ�
										for (int i = 4; i < MAPHEIGHT - 4; i++) {
											for (int j = 0; j < MAPWIDTH; j++) {
												reDrawMapPoint(i, j);
											}
										}
	
									}
								}
								if (!hasFind) {
									printChar(MAPWIDTH + 6, 29, "NO", COLOR_GRAY);
								}
							}
							break;
						case 30://����Ϊ�ؿ���ͼ: �Զ����
							printChar(MAPWIDTH + 2, 30, "������...    ", COLOR_RED);//�����Ӧ�˵�
							
							for (int i = 1; i < 10; i++) {
								int nFileNum = i;
								stringstream stream;
								stream << nFileNum;
								string  strFile =  stream.str();
								strFile.insert(0, "map\\"); //ƴ���ļ�·��
								const char * pFilename = strFile.c_str();
								FILE * pFile;
								errno_t errNum = fopen_s(&pFile, pFilename, "rb");
					
								if (errNum != 0) {//�ļ�������
									FILE* fpFile;
									fopen_s(&fpFile, pFilename, "wb");
									//д�뾲̬��ͼ����
									initHeart(); //home
									fwrite(m_nMap, sizeof(int), MAPWIDTH* MAPHEIGHT, fpFile);
									fclose(fpFile);
									printChar(MAPWIDTH + 2, 30, "����ɹ�...        ", COLOR_RED);
									Sleep(1000);
									printChar(MAPWIDTH + 2, 30, "������Ϊȱʧ�ؿ���", COLOR_GRAY);//�����Ӧ�˵�
									break;
								}
								else {
									fclose(pFile);
								}
							
							}
							break;
						case 31://ǿ�Ʊ���Ϊ�ؿ� nLevelPass
							printChar(MAPWIDTH + 2, 31, "������...        ", COLOR_RED);
							//ƴ���ļ�·��
							{
								stringstream stream;
								stream << nLevelPass;
								string  strLevel = stream.str();
								strLevel.insert(0, "map\\");

								FILE* fpFile;
								fopen_s(&fpFile, (char *)strLevel.c_str(), "wb");
								//д�뾲̬��ͼ����
								initHeart(); //home
								fwrite(m_nMap, sizeof(int), MAPWIDTH* MAPHEIGHT, fpFile);
								fclose(fpFile);
							}
							printChar(MAPWIDTH + 2, 31, "����ɹ�...        ", COLOR_RED);
							Sleep(1000);
							printChar(MAPWIDTH + 2, 31, "������:ָ���ؿ��š�", COLOR_GRAY);//�����Ӧ�˵�
							break;
						case 32: //���ڹؿ�����
							if (mousePosX == MAPWIDTH + 5) {//��С�ؿ����֣�
								nLevelPass--;
								nLevelPass = nLevelPass >= 1 ? nLevelPass : 1;
								
							}
							else if (mousePosX == MAPWIDTH + 7) { //����ؿ�����
								nLevelPass++;
								nLevelPass = nLevelPass <= 36 ? nLevelPass : 36;
							}
							{
							printChar(MAPWIDTH + 6, 32, "  ", COLOR_GRAY); //������֣� ��ֹ10�������ֲ���
							stringstream stream;
							stream << nLevelPass;
							string  strLevel = stream.str();
							printChar(MAPWIDTH + 6, 32, (char *)strLevel.c_str(), COLOR_GRAY);
							}
							break;
						default:
							//
							break;
						}
					}
					else if (mousePosX > 0 && mousePosX < MAPWIDTH - 1 && mousePosY > 3 && mousePosY < MAPHEIGHT - 4) {
						//����������
						if (nBrushColorNum == m_nMap[mousePosY][mousePosX]) { //ȡ��
							m_nMap[mousePosY][mousePosX] = 0;
						}
						else {
							m_nMap[mousePosY][mousePosX] = nBrushColorNum;
						}
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
				//������˫�� 
				if (mousePosX > 0 && mousePosX < MAPWIDTH - 1 && mousePosY > 3 && mousePosY < MAPHEIGHT - 4) {
					m_nMap[mousePosY][mousePosX] = nBrushColorNum;
					reDrawMapPoint(mousePosY, mousePosX);
					//�������ȡ��������ͼ
					activeScrollPainting = !activeScrollPainting;
 				}
				break;
			case MOUSE_MOVED:
				mousePosX = mer.dwMousePosition.X / 2;
				mousePosY = mer.dwMousePosition.Y;
				if (mousePosX > 0 && mousePosX < MAPWIDTH - 1 && mousePosY > 3 && mousePosY < MAPHEIGHT - 4) {
					//������
					if (activeScrollPainting) { // ������ͼ
						m_nMap[mousePosY][mousePosX] = nBrushColorNum;
						reDrawMapPoint(mousePosY, mousePosX);
					}
				}

				break;
			}
		}

		//��ǽ��ɫ�����������˻�����ͼ
		for (int row = 0; row < MAPHEIGHT; row++) {
			for (int col = 0; col < MAPWIDTH; col++) {
				if (row == 0 || row == MAPHEIGHT - 1 || col == 0 || col == MAPWIDTH - 1) {
					if (activeScrollPainting) {
						CMaps::printChar(col, row, "��", COLOR_YELLOW);
						printChar(1, MAPHEIGHT, "������ͼ�ѿ���: ˫���ر�", COLOR_YELLOW);
					}
					else {
						CMaps::printChar(col, row, "��", COLOR_GRAY);
						printChar(1, MAPHEIGHT, "                                            ", COLOR_GRAY);
					}
					
				}
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
	else if (m_nMap[row][col] == MAP_HEART) {
		printChar(col, row, "��", COLOR_RED);
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

void CMaps::readStaticMapFile(int nFileNum)
{
	//�����m_nMaps;
	for (int row = 0; row < MAPHEIGHT; row++) {
		for (int col = 0; col < MAPWIDTH; col++) {
			m_nMap[row][col] = 0;
		}
	}
	//��յ�ͼ�ļ�·��
	stringstream stream;
	stream << nFileNum;
	string  strFile = stream.str();
	strFile.insert(0, "map\\"); //ƴ���ļ�·��
	const char * pFilename = strFile.c_str();
	FILE * pFile;
	fopen_s(&pFile, pFilename, "rb");
	//��ͼg_nMap����
	fread_s(m_nMap, sizeof(int)*MAPWIDTH*MAPHEIGHT, sizeof(int), MAPWIDTH*MAPHEIGHT, pFile);
	fclose(pFile);
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

void  CMaps::printChar(int posX, int posY, char* pszChar, WORD wArr)
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


