#include "Tank.h"


CTank::CTank(CMaps* pMaps)
{
	__initTankShapeModel();
	m_pMaps = pMaps;
}

CTank::~CTank()
{
}

void CTank::__initTankShapeModel()
{
	int tankShape[4][3][3] = {
		{//up = 0
			{ 0, 1, 0 },
			{ 1, 1, 1 },
			{ 1, 0, 1 }
		},
		{//right = 1
			{ 1, 1, 0 },
			{ 0, 1, 1 },
			{ 1, 1, 0 }
		},
		{ //down = 2
			{ 1, 0, 1 },
			{ 1, 1, 1 },
			{ 0, 1, 0 }
		},
		{ //left
			{ 0, 1, 1 },
			{ 1, 1, 0 },
			{ 0, 1, 1 }
		}
	};

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				m_tankShape[i][j][k] = tankShape[i][j][k];
			}
		}
	}
}


void CTank::initNpcTank(int Count)
{
	srand((unsigned int)time(NULL));
	Count = Count > 18 ? 18 : Count;
	for (int nNum = 0; nNum < Count; nNum++) {
		TANK*  pTank = (TANK*)malloc(sizeof(TANK));
		pTank->isNPC = true;
		pTank->color = COLOR_YELLOW;
		pTank->dir = DIR_DOWN;
		pTank->maxHP = 60+ nNum * 20;
		pTank->curHP = pTank->maxHP;
		pTank->maxSpeed = 50 + rand()%50;
		pTank->curSpeed = pTank->maxSpeed;
		pTank->oldSpeed = 0;
		pTank->nKill = 0;
		pTank->nDie = 0;
		pTank->nlife = 2;
		pTank->nlevel = 1;
		pTank->last_move_time = 0;
		pTank->last_shoot_time = 0;
		pTank->isAlive = true;
		pTank->nShape = nNum % 4 + 1;
		//����
		int X;
		int Y = 2;
		if (nNum % 2 == 0) { //���������
			X = 2 + (nNum / 2) * 3;
		}
		else { //�������ұ�
			X = MAPWIDTH - 3 - ((nNum-1)/2)*3;
		}
		pTank->posX = X;
		pTank->posY = Y;
		pTank->initPosX = X;
		pTank->initPosY = Y;
		m_vecTank.push_back(pTank);
		drawTank(m_vecTank.size() - 1, true);
	}
}

void CTank::autoRunNpcTank(CBullet& bullets)
{
	srand((unsigned int)time(NULL));
	POS endHeart;//Ŀ�� home
	endHeart.posX = MAPWIDTH / 2 - 1;
	endHeart.posY = MAPHEIGHT - 3;
	POS player01;
	player01.posX = m_vecTank[0]->posX;
	player01.posY = m_vecTank[0]->posY;

	POS player02;
	player02.posX = m_vecTank[1]->posX;
	player02.posY = m_vecTank[1]->posY;

	POS targetPos;

	bool bToHeart = false;
	bool bToPlayer01 = false;
	bool bToPlayer02 = false;
	static CAstar Astar;
	for (unsigned int index = 0; index < m_vecTank.size(); index++) {
		if (m_vecTank[index]->isNPC && m_vecTank[index]->isAlive) {
			
			POS start;
			start.posX = m_vecTank[index]->posX;
			start.posY = m_vecTank[index]->posY;
			if (bToHeart == false) {
				targetPos = endHeart;
				bToHeart = true;
			}
			else if (m_vecTank[0]->isAlive && bToPlayer01 == false) {
				targetPos = player01;
				bToPlayer01 = true;
			}
			else if (!m_vecTank[1]->isNPC && m_vecTank[1]->isAlive) {
				targetPos = player02;
				bToPlayer02 = true;
			}
			else {
				targetPos = player01;
			}
			
			int nDir = -1;
			//����
			if (true) { // true--�Զ�Ѱ·
				targetPos = player01;
				if (Astar.searchPath(*m_pMaps, start, targetPos)) {
					nDir = Astar.getMoveDir(start);
				}
				if (nDir == -1) {
					continue;
				}
			}
			if(nDir == -1){
				nDir = rand() % 4;
			}
			
			
			moveTank(nDir, index); //�ƶ�
			bullets.shootBullet(m_vecTank, index); //���
		}
	}


}

void CTank::initPlayerTank(int Count)
{
	srand((unsigned int)time(NULL));
	for (int i = 0; i < Count; i++) {
		TANK*  pTank = (TANK*)malloc(sizeof(TANK));
		pTank->isNPC = false;
		pTank->posX = MAPWIDTH / 2 - 4 + 8* i;
		pTank->posY = MAPHEIGHT - 3;
		pTank->initPosX = pTank->posX;
		pTank->initPosY = pTank->posY;
		pTank->color = COLOR_RED;
		pTank->dir = DIR_UP;
		pTank->maxHP = 100; 
		pTank->curHP = 100;
		pTank->maxSpeed = 100+rand()%100;//��Ҫ����80
		pTank->curSpeed = pTank->maxSpeed;
		pTank->oldSpeed = 0;
		pTank->nKill = 0;
		pTank->nDie = 0;
		pTank->nlife = 5;  //------------------------------------test
		pTank->nlevel = 1;
		pTank->last_move_time = 0;
		pTank->last_shoot_time = 0;
		pTank->isAlive = true;
		pTank->nShape = TANK_SHAPE5;

		m_vecTank.push_back(pTank);
		drawTank(m_vecTank.size() - 1, true);
	}
	

	
}

//�˺���ͬ��m_pMaps->m_pTankMap����
void CTank::drawTank( int index, bool isShow)
{
	srand((unsigned int)time(NULL));
	//��ʼ���ٶ�
	m_vecTank[index]->curSpeed = m_vecTank[index]->maxSpeed;
	int posX = m_vecTank[index]->posX;
	int posY = m_vecTank[index]->posY;
	int dirNum = m_vecTank[index]->dir;
	for (int row = posY - 1; row <= posY + 1; row++) {
		for (int col = posX - 1; col <= posX + 1; col++) {
			if (m_tankShape[dirNum][row - posY + 1][col - posX + 1] == 1) {
				if (m_pMaps->m_nMap[row][col] == MAP_TREE) {
					//����Ҫ����̹�ˣ� ����ʾҲ������
				}
				else if (isShow) {
					char * shape;
					switch (m_vecTank[index]->nShape) {
					case TANK_SHAPE1:
						shape = "��";
						break;
					case TANK_SHAPE2:
						shape = "��";
						break;
					case TANK_SHAPE3:
						shape = "��";
						break;
					case TANK_SHAPE4:
						shape = "��";
						break;
					case TANK_SHAPE5:
						shape = "��";
						break;
					default:
						shape = "��";
						
					}
					m_pMaps->printChar(col, row, shape, m_vecTank[index]->color);
					m_pMaps->m_pTankMap[row][col] = m_vecTank[index]; // ͬ��̹�˵�ͼ
															 //�����ʾλ����ֲ��
					if (m_pMaps->m_nMap[row][col] == MAP_GRASS || m_pMaps->m_nMap[row][col] == MAP_ICE) {//ֲ�ﱻ�ƻ�
						m_pMaps->m_vecDamagedPlant.push_back({ col, row });   //ͬ������ֲ���ͼ
						
						if (m_pMaps->m_nMap[row][col] == MAP_GRASS) {//����
							m_vecTank[index]->curSpeed = int(m_vecTank[index]->maxSpeed/(rand()%4 +1));
						}
						else {//����
							m_vecTank[index]->curSpeed = int(m_vecTank[index]->maxSpeed + m_vecTank[index]->maxSpeed/(rand()%4+1));
						}
					}
					else if (m_pMaps->m_nMap[row][col] == MAP_HEART) {//��������Ƿ���ѹ
						m_pMaps->m_isHeartBroken = true;
					}
					
					
				}
				else {
					m_pMaps->printChar(col, row, "  ", COLOR_BLACK);
					m_pMaps->m_pTankMap[row][col] = NULL;
					if (m_pMaps->m_nMap[row][col] == MAP_GRASS || m_pMaps->m_nMap[row][col] == MAP_ICE) {//ֲ�ﱻ�ƻ�
						m_pMaps->m_vecDamagedPlant.push_back({ col, row });   //ͬ������ֲ���ͼ
					}
				}
			}
		}
	}

}

void CTank::initDrawAllTank(bool isShow)
{
	for (unsigned int i = 0; i < m_vecTank.size(); i++) {
		drawTank(i, true);
	}
}

bool CTank::moveTank(int nDirNum, int index = 0)
{
	if (m_vecTank[index]->isAlive == false) {
		return false;
	}
	//������
	int nDestPosX = m_vecTank[index]->posX;
	int nDestPosY = m_vecTank[index]->posY;
	int nDestDir = m_vecTank[index]->dir;
	//�ж��ƶ��������������겻��
	int  blockNumSum = MAP_STONE + MAP_BRICK + MAP_RIVER;
	int changeValue = m_vecTank[index]->dir - nDirNum;
	if (changeValue == 0) { //ǰ�����޸�����,���򲻱�
		//�ٶ����ã�ֻ��ǰ�������ٶ�
		clock_t start_time = m_vecTank[index]->last_move_time;
		clock_t end_time = clock();
		if (end_time - start_time < (200- m_vecTank[index]->curSpeed)*4) {
			return false;
		}
		m_vecTank[index]->last_move_time = end_time;
		//��ʼ�ж�
		switch (m_vecTank[index]->dir) {
		case DIR_UP:
			if ((m_pMaps->m_nMap[nDestPosY - 1][nDestPosX - 1] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY - 2][nDestPosX] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY - 1][nDestPosX + 1] & blockNumSum) != 0
				|| m_pMaps->m_pTankMap[nDestPosY - 1][nDestPosX - 1] != NULL || m_pMaps->m_pTankMap[nDestPosY - 2][nDestPosX] != NULL || m_pMaps->m_pTankMap[nDestPosY - 1][nDestPosX + 1] != NULL) {
				return false;//������--Ŀ��λ����ǽ
			}
			nDestPosY--;
			break;
		case DIR_RIGHT:
			if ((m_pMaps->m_nMap[nDestPosY - 1][nDestPosX + 1] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY][nDestPosX + 2] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY + 1][nDestPosX + 1] & blockNumSum) != 0
				|| m_pMaps->m_pTankMap[nDestPosY - 1][nDestPosX + 1] != NULL || m_pMaps->m_pTankMap[nDestPosY][nDestPosX + 2] != NULL || m_pMaps->m_pTankMap[nDestPosY + 1][nDestPosX + 1] != NULL) {
				return false;//������--Ŀ��λ����ǽ
			}
			nDestPosX++;
			break;
		case DIR_DOWN:
			if ((m_pMaps->m_nMap[nDestPosY + 1][nDestPosX - 1] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY + 2][nDestPosX] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY + 1][nDestPosX + 1] & blockNumSum) != 0
				|| m_pMaps->m_pTankMap[nDestPosY + 1][nDestPosX - 1] != 0 || m_pMaps->m_pTankMap[nDestPosY + 2][nDestPosX] != 0 || m_pMaps->m_pTankMap[nDestPosY + 1][nDestPosX + 1] != 0) {
				return false;//������--Ŀ��λ����ǽ
			}
			nDestPosY++;
			break;
		case DIR_LEFT:
			if ((m_pMaps->m_nMap[nDestPosY - 1][nDestPosX - 1] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY][nDestPosX - 2] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY + 1][nDestPosX - 1] & blockNumSum) != 0
				|| m_pMaps->m_pTankMap[nDestPosY - 1][nDestPosX - 1] != 0 || m_pMaps->m_pTankMap[nDestPosY][nDestPosX - 2] != 0 || m_pMaps->m_pTankMap[nDestPosY + 1][nDestPosX - 1] != 0) {
				return false;//������-Ŀ��λ����ǽ
			}
			nDestPosX--;
			break;
		default:
			printf("̹��ǰ���������dirNum����0123���� moveTank\n");
			return false;
		}
	}
	else if (changeValue == -1 || changeValue == -3 || changeValue == 1 || changeValue == 3) { //�����ƶ�: ֻ�ı䷽�����Ҽ��β�����������壩
		switch (m_vecTank[index]->dir) {
		case DIR_UP:
			if ((m_pMaps->m_nMap[nDestPosY - 1][nDestPosX - 1] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY + 1][nDestPosX] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY - 1][nDestPosX + 1] & blockNumSum) != 0
				|| m_pMaps->m_pTankMap[nDestPosY - 1][nDestPosX - 1] != 0 || m_pMaps->m_pTankMap[nDestPosY + 1][nDestPosX] != 0 || m_pMaps->m_pTankMap[nDestPosY - 1][nDestPosX + 1] != 0) {
				return false;//������--Ŀ��λ����ǽ
			}
			break;
		case DIR_RIGHT:
			if ((m_pMaps->m_nMap[nDestPosY - 1][nDestPosX + 1] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY][nDestPosX - 1] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY + 1][nDestPosX + 1] & blockNumSum) != 0
				|| m_pMaps->m_pTankMap[nDestPosY - 1][nDestPosX + 1] != 0 || m_pMaps->m_pTankMap[nDestPosY][nDestPosX - 1] != 0 || m_pMaps->m_pTankMap[nDestPosY + 1][nDestPosX + 1] != 0) {
				return false;//������--Ŀ��λ����ǽ
			}
			break;
		case DIR_DOWN:
			if ((m_pMaps->m_nMap[nDestPosY + 1][nDestPosX - 1] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY - 1][nDestPosX] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY + 1][nDestPosX + 1] & blockNumSum) != 0
				|| m_pMaps->m_pTankMap[nDestPosY + 1][nDestPosX - 1] != 0 || m_pMaps->m_pTankMap[nDestPosY - 1][nDestPosX] != 0 || m_pMaps->m_pTankMap[nDestPosY + 1][nDestPosX + 1] != 0) {
				return false;//������--Ŀ��λ����ǽ
			}
			break;
		case DIR_LEFT:
			if ((m_pMaps->m_nMap[nDestPosY - 1][nDestPosX - 1] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY][nDestPosX + 1] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY + 1][nDestPosX - 1] & blockNumSum) != 0
				|| m_pMaps->m_pTankMap[nDestPosY - 1][nDestPosX - 1] != 0 || m_pMaps->m_pTankMap[nDestPosY][nDestPosX + 1] != 0 || m_pMaps->m_pTankMap[nDestPosY + 1][nDestPosX - 1] != 0) {
				return false;//������-Ŀ��λ����ǽ
			}
			break;
		default:
			printf("̹�������ҷ����ƶ�����dirNum����0123���� moveTank\n");
			return false;
		}
		nDestDir = nDirNum;
	}
	else if (changeValue == 2 || changeValue == -2) { //�����ƶ�: ֻ�ı����꣬���򲻱䣨β�����������������壩
		//��������˳ʱ��ת
		int nTryDir = m_vecTank[index]->dir + 1;
		nTryDir = nTryDir <= 3 ? nTryDir : 0;
		if (moveTank(nTryDir, index)) { //�ƶ��ɹ��� ��˳ʱ���ƶ�һ��
			nTryDir++;
			nTryDir = nTryDir <= 3 ? nTryDir : 0;
			if (moveTank(nTryDir, index)) { //���γɹ�
				return true;
			}
			else {//��ʱ�룬��ԭ����
				nTryDir--;
				nTryDir = nTryDir >= 0 ? nTryDir : 0;
				moveTank(nTryDir, index);
				//ֻ�ܵ�����
			}
		}

		//��ʼ�������ж�
		switch (m_vecTank[index]->dir) {
		case DIR_UP:
			if ((m_pMaps->m_nMap[nDestPosY + 2][nDestPosX - 1] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY + 1][nDestPosX] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY + 2][nDestPosX + 1] & blockNumSum) != 0
				|| m_pMaps->m_pTankMap[nDestPosY + 2][nDestPosX - 1] != 0 || m_pMaps->m_pTankMap[nDestPosY + 1][nDestPosX] != 0 || m_pMaps->m_pTankMap[nDestPosY + 2][nDestPosX + 1] != 0) {
				return false;//������--Ŀ��λ����ǽ
			}
			nDestPosY++;
			break;
		case DIR_RIGHT:
			if ((m_pMaps->m_nMap[nDestPosY - 1][nDestPosX - 2] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY][nDestPosX - 1] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY + 1][nDestPosX - 2] & blockNumSum) != 0
				|| m_pMaps->m_pTankMap[nDestPosY - 1][nDestPosX - 2] != 0 || m_pMaps->m_pTankMap[nDestPosY][nDestPosX - 1] != 0 || m_pMaps->m_pTankMap[nDestPosY + 1][nDestPosX - 2] != 0) {
				return false;//������--Ŀ��λ����ǽ
			}
			nDestPosX--;
			break;
		case DIR_DOWN:
			if ((m_pMaps->m_nMap[nDestPosY - 2][nDestPosX - 1] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY - 1][nDestPosX] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY - 2][nDestPosX + 1] & blockNumSum) != 0
				|| m_pMaps->m_pTankMap[nDestPosY - 2][nDestPosX - 1] != 0 || m_pMaps->m_pTankMap[nDestPosY - 1][nDestPosX] != 0 || m_pMaps->m_pTankMap[nDestPosY - 2][nDestPosX + 1] != 0) {
				return false;//������--Ŀ��λ����ǽ
			}
			nDestPosY--;
			break;
		case DIR_LEFT:
			if ((m_pMaps->m_nMap[nDestPosY - 1][nDestPosX + 2] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY][nDestPosX + 1] & blockNumSum) != 0 || (m_pMaps->m_nMap[nDestPosY + 1][nDestPosX + 2] & blockNumSum) != 0
				|| m_pMaps->m_pTankMap[nDestPosY - 1][nDestPosX + 2] != 0 || m_pMaps->m_pTankMap[nDestPosY][nDestPosX + 1] != 0 || m_pMaps->m_pTankMap[nDestPosY + 1][nDestPosX + 2] != 0) {
				return false;//������-Ŀ��λ����ǽ
			}
			nDestPosX++;
			break;
		default:
			printf("̹�����ҷ������dirNum����0123���� moveTank\n");
			return false;
		}
	}
	//�����ƶ�
	drawTank(index, false); //����
							//�޸�����
	m_vecTank[index]->posX = nDestPosX;
	m_vecTank[index]->posY = nDestPosY;
	m_vecTank[index]->dir = nDestDir;
	//����
	drawTank(index, true);
	//����?
	return true;
}

int  CTank::judgeAlive()
{
	int playerNum = 0;
	int NpcNum = 0;
	for (unsigned int i = 0; i < m_vecTank.size(); i++) {
		if (m_vecTank[i]->curHP > 0 || m_vecTank[i]->isAlive == false) { //�����ж�:��Ѫ��������
			if (m_vecTank[i]->isNPC && m_vecTank[i]->curHP > 0) {
				NpcNum++;
			}
			else if(m_vecTank[i]->curHP > 0){
				playerNum++;
			}
			continue;
		}
		if (m_vecTank[i]->nlife >= 1) { //��life��Ѫ
			m_vecTank[i]->curHP = m_vecTank[i]->maxHP;
			m_vecTank[i]->nlife = m_vecTank[i]->nlife - 1;
			m_vecTank[i]->nDie = m_vecTank[i]->nDie + 1;
			if (m_vecTank[i]->isNPC) {
				NpcNum++;
			}
			else {
				playerNum++;
			}
			drawTank(i, false);
			m_vecTank[i]->posX = m_vecTank[i]->initPosX;
			m_vecTank[i]->posY = m_vecTank[i]->initPosY;
			continue;
		}
		//����
		m_vecTank[i]->isAlive = false;
		drawTank(i, false);
	}

	//�ж�˫������̹������
	if (playerNum == 0) { //ʧ��
		return -1;  
	}
	else if (NpcNum == 0) { //ʤ��
		return 1;
	}
	else {		// ����
		return 0;  
	}
}


void CTank::showTankInfo()
{
	stringstream stream;
	for (unsigned int index = 0; index < m_vecTank.size(); index++) {
		
		int posX = MAPWIDTH + 1;
		int posY = 2 + 4 * index;
		int dirNum = 0;
		//��̹��
		for (int row = posY - 1; row <= posY + 1; row++) {
			for (int col = posX - 1; col <= posX + 1; col++) {
				if (m_tankShape[dirNum][row - posY + 1][col - posX + 1] == 1) {
					char * shape;
					switch (m_vecTank[index]->nShape) {
					case TANK_SHAPE1:
						shape = "��";
						break;
					case TANK_SHAPE2:
						shape = "��";
						break;
					case TANK_SHAPE3:
						shape = "��";
						break;
					case TANK_SHAPE4:
						shape = "��";
						break;
					case TANK_SHAPE5:
						shape = "��";
						break;
					default:
						shape = "��";

					}
					if (!m_vecTank[index]->isAlive){ //����
							m_pMaps->printChar(col, row, shape, COLOR_GRAY);
					}
					else {
						m_pMaps->printChar(col, row, shape, m_vecTank[index]->color);
					}
				}
			}
		}
		
		//��ʾ̹������ֵ
		stream.clear();
		stream << m_vecTank[index]->curHP;
		string strHp = stream.str();
		strHp.append(" ");
		for (int i = 0; i < (int)(m_vecTank[index]->curHP/20); i++) {
			strHp.append("��");
		}
		if (m_vecTank[index]->isAlive) {
			CMaps::printChar(posX + 1, posY - 1, "--------------", COLOR_GREEN);
			CMaps::printChar(posX + 1, posY - 1, (char *)strHp.c_str(), COLOR_GREEN);
		}
		else {
			CMaps::printChar(posX + 1, posY - 1, "              ", COLOR_GRAY);
		}
		


		//��ʾ̹����������
		stream.clear();
		stream.str(""); //��ֹ�ڴ��ۻ�
		int nLife = m_vecTank[index]->nlife;
		stream << nLife;
		string  strLife= stream.str();
		strLife.insert(0, " x ");
		if (m_vecTank[index]->isAlive) {
			CMaps::printChar(posX + 2, posY, "      ", m_vecTank[index]->color);
			CMaps::printChar(posX + 2, posY, (char *)strLife.c_str(), m_vecTank[index]->color);
		}
		else {
			CMaps::printChar(posX + 2, posY, "      ", COLOR_GRAY);
		}    

		


		//��ʾ�ٶ�
		stream.clear();
		stream.str(""); //��ֹ�ڴ��ۻ�
		int nSpeed = m_vecTank[index]->curSpeed;
		stream << nSpeed;
		string  strSpeed= stream.str();
		strSpeed.insert(0, "  ");
		strSpeed.append(" Km/h");
		if (m_vecTank[index]->isAlive) {
			CMaps::printChar(posX + 2, posY + 1, "------", m_vecTank[index]->color);
			CMaps::printChar(posX + 2, posY + 1, (char *)strSpeed.c_str(), COLOR_WHITE);
		}
		else {
			CMaps::printChar(posX + 2, posY + 1, "          ", COLOR_GRAY);
		}

		stream.str(""); //��ֹ�ڴ��ۻ�
	}
}


